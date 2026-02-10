#pragma once

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>

namespace pyCGAL::wrap::utils {

template <typename Surface_mesh, typename Edge_pmap>
auto collect_edges_vertices(const Surface_mesh& mesh,
                            const Edge_pmap edge_map) {
  using Vertex_index = typename Surface_mesh::Vertex_index;
  std::vector<Vertex_index> vertices;
  for (auto&& e : mesh.edges()) {
    if (edge_map[e]) {
      vertices.push_back(mesh.vertex(e, 0));
      vertices.push_back(mesh.vertex(e, 1));
    }
  }
  std::sort(begin(vertices), end(vertices));
  vertices.erase(std::unique(begin(vertices), end(vertices)), end(vertices));
  vertices.shrink_to_fit();
  return vertices;
}
/** \returns the number of components */
template <typename Surface_mesh>
int collect_connected_cells_and_components(
    const Surface_mesh& mesh, const typename Surface_mesh::Vertex_index v,
    std::vector<typename Surface_mesh::Face_index>& cells,
    std::vector<int>& components,
    const typename Surface_mesh::template Property_map<
        typename Surface_mesh::Edge_index, bool>& edge_is_constrained) {
  // at least one edge must be constrained otherwise the algorithm will fail on
  // borders possibly detecting 2 components
  assert([&]() {
    for (auto&& h : CGAL::halfedges_around_source(v, mesh)) {
      if (edge_is_constrained[mesh.edge(h)]) return true;
    }
    return false;
  }());
  int comp_id = 0;
  for (auto&& h : CGAL::halfedges_around_source(v, mesh)) {
    if (mesh.is_border(h)) {
      ++comp_id;
      continue;
    }
    components.push_back(comp_id);
    cells.push_back(mesh.face(h));
    if (edge_is_constrained[mesh.edge(h)]) {
      ++comp_id;
    }
  }
  if (comp_id == 0) {
    return 1;
  }
  // we numbered components starting at 1
  for (auto& component : components) {
    if (component == comp_id) {
      component = 0;
    }
  }
  return comp_id;
}

// We cannot use directly boost::graph because we must remove cells along finite
// fault borders
template <typename Surface_mesh, typename Edge_pmap>
void pack_cells_along_edges(Surface_mesh& mesh,
                            const Edge_pmap edge_is_constrained,
                            const std::string& pack_property_name) {
  using Face_index = typename Surface_mesh::Face_index;

  const auto vertices = collect_edges_vertices(mesh, edge_is_constrained);

  std::vector<Face_index> cells;
  std::vector<Face_index> v_cells;
  std::vector<int> v_comps_ids;
  std::vector<std::vector<std::vector<Face_index>>> v_comps{
      vertices.size()};  // connected components around v
  // Compute local connected components around each vertex
  int maxn = 0;
  for (std::size_t i = 0; i != vertices.size(); ++i) {
    const auto n = collect_connected_cells_and_components(
        mesh, vertices[i], v_cells, v_comps_ids, edge_is_constrained);
    assert(n > 0);
    maxn = std::max(n, maxn);
    if (n > 1) {
      std::copy(begin(v_cells), end(v_cells), back_inserter(cells));
      auto& local_components = v_comps[i];
      local_components.resize(n);

      for (std::size_t j = 0; j < v_cells.size(); ++j) {
        local_components[v_comps_ids[j]].push_back(v_cells[j]);
      }
    }
    v_cells.clear();
    v_comps_ids.clear();
  }
  std::sort(begin(cells), end(cells));
  cells.erase(std::unique(begin(cells), end(cells)), end(cells));

  assert(std::is_sorted(begin(cells), end(cells)));

  // using std::vector to store vertices and edges
  using Graph =
      boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS>;
  Graph graph{size(cells)};

  // FIXME: could be speed up by a map between rank with repetition and rank
  // without
  //        so that the rank of a cell is never searched for
  std::vector<std::vector<int>> local_ranks;  // Working array
  local_ranks.resize(maxn);
  // Compute "global" connected components around discontinuities
  for (auto&& local_components : v_comps) {
    const auto nc = local_components.size();
    for (auto&& ranks : local_ranks) {
      ranks.clear();
    }
    auto pranks = begin(local_ranks);
    for (auto&& comp : local_components) {
      for (auto&& ch : comp) {
        auto it = std::lower_bound(begin(cells), end(cells), ch);
        assert(it != end(cells) && *it == ch);
        // use the rank of ch in cells to identify graph node
        pranks->push_back(static_cast<int>(std::distance(begin(cells), it)));
      }
      ++pranks;
    }
    for (auto&& comp : local_ranks) {
      // Add edges between all cells of the local component
      for (std::size_t i = 0; i != comp.size(); ++i) {
        for (std::size_t j = i + 1; j != comp.size(); ++j) {
          add_edge(comp[i], comp[j], graph);
        }
      }
    }
  }

  assert(num_vertices(graph) == cells.size());
  std::vector<int> pack_ids;
  pack_ids.resize(cells.size());
  connected_components(graph, pack_ids.data());

  auto [pack, created] =
      mesh.template add_property_map<Face_index, int>(pack_property_name, 0);
#ifndef NDEBUG
  if (!created) {
    std::cerr << "WARNING: the property " << pack_property_name
              << " is overwritten!" << std::endl;
  }
#endif
  auto pi = begin(pack_ids);
  for (auto&& ch : cells) {
    pack[ch] = (*pi) + 1;
    ++pi;
  }
}

}  // namespace pyCGAL::wrap::utils
