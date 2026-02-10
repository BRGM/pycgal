#pragma once

#include "c3t3_vertex_connected_component.h"

namespace pyCGAL {
namespace wrap {
namespace utils {

// FIXME: we could select the facet indices
template <typename C3t3>
auto collect_facets_vertices(const C3t3& c3t3) {
  using Vertex_handle = typename C3t3::Vertex_handle;
  std::vector<Vertex_handle> vertices;
  for (auto fit = c3t3.facets_in_complex_begin();
       fit != c3t3.facets_in_complex_end(); ++fit) {
    auto&& [ch, vf] = *fit;
    for (int i = vf + 1; i != vf + 4; ++i) {
      vertices.push_back(ch->vertex(i % 4));
    }
  }
  std::sort(begin(vertices), end(vertices));
  vertices.erase(std::unique(begin(vertices), end(vertices)), end(vertices));
  vertices.shrink_to_fit();
  return vertices;
}

template <typename C3t3>
inline auto sort_and_unique(const C3t3& c3t3,
                            std::vector<typename C3t3::Cell_handle>& cells) {
  std::sort(begin(cells), end(cells));
  cells.erase(std::unique(begin(cells), end(cells)), end(cells));
  // FIXME: C++ 20 bring erase_if
  cells.erase(remove_if(begin(cells), end(cells),
                        [&c3t3](auto&& ch) { return !c3t3.is_in_complex(ch); }),
              end(cells));
  cells.shrink_to_fit();
}

template <typename C3t3, typename Vertices>
auto collect_incident_cells(const C3t3& c3t3, Vertices&& vertices) {
  using Cell_handle = typename C3t3::Cell_handle;
  using Triangulation = typename C3t3::Triangulation;

  const Triangulation& tr = c3t3.triangulation();
  std::vector<Cell_handle> cells;

  for (auto&& v : vertices) {
    tr.finite_incident_cells(v, back_inserter(cells));
  }
  sort_and_unique(c3t3, cells);
  return cells;
}

template <typename C3t3>
auto collect_facet_cells(const C3t3& c3t3) {
  return collect_incident_cells(c3t3, collect_facets_vertices(c3t3));
}

// We cannot use directly boost::graph because we must remove cells along finite
// fault borders
template <typename C3t3>
auto pack_cells_along_facets(const C3t3& c3t3) {
  using Vertex_handle = typename C3t3::Vertex_handle;
  using Cell_handle = typename C3t3::Cell_handle;

  const auto vertices = collect_facets_vertices(c3t3);

  std::vector<Cell_handle> cells;
  std::vector<Cell_handle> v_cells;
  std::vector<int> v_comps_ids;
  std::vector<std::vector<std::vector<Cell_handle>>> v_comps{
      vertices.size()};  // connected components around v
  // Compute local connected components around each vertex
  int maxn = 0;
  for (std::size_t i = 0; i != vertices.size(); ++i) {
    const auto n = collect_connected_cells_and_components(c3t3, vertices[i],
                                                          v_cells, v_comps_ids);
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
  }
  // FIXME: we could skip the in_complex test
  sort_and_unique(c3t3, cells);

  // using std::vector to store vertices and edges
  using Graph =
      boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS>;
  Graph graph{size(cells)};

  assert(std::is_sorted(begin(cells), end(cells)));
  // FIXME: could be speed up by a map between rank with repetition and rank
  // without
  //        so that the rank of a cell is never searched for
  std::vector<std::vector<int>> local_ranks;
  local_ranks.resize(maxn);
  // Compute "global" connected components around discontinuities
  for (auto&& local_components : v_comps) {
    const auto nc = local_components.size();
    for (auto&& ranks : local_ranks) ranks.clear();
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

  // FIXME: to be changed
  std::unordered_map<Cell_handle, int> packs;
  auto pi = begin(pack_ids);
  for (auto&& ch : cells) {
    packs.insert({ch, (*pi) + 1});
    ++pi;
  }
  return packs;
}

}  // namespace utils
}  // namespace wrap
}  // namespace pyCGAL
