#pragma once

#include <pybind11/numpy.h>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>

namespace pyCGAL {
namespace wrap {
namespace utils {

template <typename C3t3>
auto connected_components_to_array(const C3t3& c3t3) {
  using Triangulation = typename C3t3::Triangulation;
  using Cell_handle = typename Triangulation::Cell_handle;
  using Facet = typename Triangulation::Facet;

  // using std::vector to store vertices and edges
  using Graph =
      boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS>;
  Graph graph{c3t3.number_of_cells_in_complex()};

  int k = 0;  // cell = graph vertex
  std::unordered_map<Cell_handle, int> gv;
  for (auto cit = c3t3.cells_in_complex_begin();
       cit != c3t3.cells_in_complex_end(); ++cit, ++k) {
    gv.insert({cit, k});
  }

  k = 0;
  const Triangulation& tr = c3t3.triangulation();
  for (auto cit = c3t3.cells_in_complex_begin();
       cit != c3t3.cells_in_complex_end(); ++cit, ++k) {
    for (int i = 0; i < 4; ++i) {
      Facet f{cit, i};
      if (!c3t3.is_in_complex(f))
        add_edge(k, gv[tr.mirror_facet(f).first], graph);
    }
  }

  assert(num_vertices(graph) == c3t3.number_of_cells_in_complex());
  py::array_t<int, py::array::c_style> component{
      static_cast<py::ssize_t>(c3t3.number_of_cells_in_complex())};

  int nbcomps = connected_components(graph, component.mutable_data());
  return py::make_tuple(nbcomps, component);
}

}  // namespace utils
}  // namespace wrap
}  // namespace pyCGAL
