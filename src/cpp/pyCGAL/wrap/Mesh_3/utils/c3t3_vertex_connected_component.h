#pragma once

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>

namespace pyCGAL {
namespace wrap {
namespace utils {

template <typename C3t3>
void collect_connected_cells(const C3t3& c3t3,
                             const typename C3t3::Vertex_handle v,
                             std::vector<typename C3t3::Cell_handle>& cells) {
  using Triangulation = typename C3t3::Triangulation;
  using Cell_handle = typename C3t3::Cell_handle;

  const Triangulation& tr = c3t3.triangulation();

  assert(!tr.is_infinite(v));
  tr.finite_incident_cells(v, back_inserter(cells));
  // FIXME: C++ 20 bring erase_if
  cells.erase(remove_if(begin(cells), end(cells),
                        [&c3t3](auto&& ch) { return !c3t3.is_in_complex(ch); }),
              end(cells));
  // assert(all_of(begin(incident_cells), end(incident_cells), [v](auto&& ch) {
  //   return ch->vertex(0) == v || ch->vertex(1) == v || ch->vertex(2) == v ||
  //          ch->vertex(3) == v;
  // }));
}

template <typename C3t3>
int collect_connected_cells_and_components(
    const C3t3& c3t3, const typename C3t3::Vertex_handle v,
    std::vector<typename C3t3::Cell_handle>& cells,
    std::vector<int>& components) {
  using Triangulation = typename C3t3::Triangulation;
  using Vertex_handle = typename C3t3::Vertex_handle;
  using Cell_handle = typename C3t3::Cell_handle;
  using Facet = typename Triangulation::Facet;

  const Triangulation& tr = c3t3.triangulation();

  cells.clear();
  collect_connected_cells(c3t3, v, cells);

  // using std::vector to store vertices and edges
  using Graph =
      boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS>;
  Graph graph{size(cells)};

  // cell = graph vertex
  std::unordered_map<Cell_handle, int> gv;
  int k = 0;
  for (auto&& ch : cells) {
    gv.insert({ch, k});
    ++k;
  }

  std::vector<Facet> facets;
  tr.finite_incident_facets(v, back_inserter(facets));
  // FIXME: C++ 20 bring erase_if
  facets.erase(
      remove_if(begin(facets), end(facets),
                [&c3t3, &tr](auto&& f) {
                  return !(c3t3.is_in_complex(f.first) &&
                           c3t3.is_in_complex(tr.mirror_facet(f).first));
                }),
      end(facets));
  // assert(all_of(begin(facets), end(facets),
  //               [&c3t3, &tr](auto&& f) {
  //                 return c3t3.is_in_complex(f.first) &&
  //                        c3t3.is_in_complex(tr.mirror_facet(f).first);
  //               }));
  for (auto&& f : facets) {
    assert(!tr.is_infinite(f));
    // FIXME: C++20 brings contains
    // assert(gv.contains(f.first));
    // assert(gv.contains(tr.mirror_facet(f).first));
    static_assert(std::is_same_v<decltype(f.first), Cell_handle>);
    // assert(c3t3.is_in_complex(f.first));
    // assert(c3t3.is_in_complex(tr.mirror_facet(f).first));
    assert(gv.find(f.first) != gv.end());
    assert(gv.find(tr.mirror_facet(f).first) != gv.end());
    if (!c3t3.is_in_complex(f)) {
      add_edge(gv[f.first], gv[tr.mirror_facet(f).first], graph);
    }
  }
  assert(num_vertices(graph) == cells.size());
  components.resize(cells.size());
  return connected_components(graph, components.data());
}

template <typename C3t3>
int number_of_connected_components(const C3t3& c3t3,
                                   const typename C3t3::Vertex_handle v) {
  using Cell_handle = typename C3t3::Cell_handle;

  std::vector<Cell_handle> cells;
  std::vector<int> components;
  return collect_connected_cells_and_components(c3t3, v, cells, components);
}

}  // namespace utils
}  // namespace wrap
}  // namespace pyCGAL
