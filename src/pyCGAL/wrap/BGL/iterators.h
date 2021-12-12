#pragma once

#include <CGAL/boost/graph/iterator.h>
#include <pyCGAL/typedefs.h>

#include "detail/iterators.h"

namespace pyCGAL {

namespace {

template <typename Iterator>
void wrap_iterator_range(py::module& module, const char* name) {
  py::class_<Iterator> pyclass(module, name);
  pyclass.def("__iter__", [](Iterator& self) {
    return py::make_iterator(self.begin(), self.end());
  });
}

}  // namespace

template <typename Graph>
void wrap_element(wrap::BGL::detail::iterators<Graph>, py::module& module) {
  wrap_iterator_range<
      CGAL::Iterator_range<CGAL::Halfedge_around_face_iterator<Graph>>>(
      module, "Halfedge_around_face_iterator");
  module.def("halfedges_around_face", &CGAL::halfedges_around_face<Graph>);

  wrap_iterator_range<
      CGAL::Iterator_range<CGAL::Vertex_around_face_iterator<Graph>>>(
      module, "Vertex_around_face_iterator");
  module.def("vertices_around_face", &CGAL::vertices_around_face<Graph>);
  module.def("vertices_around_face",
             [](const typename Graph::Face_index& face, const Graph& graph) {
               return CGAL::vertices_around_face(graph.halfedge(face), graph);
             });

  wrap_iterator_range<
      CGAL::Iterator_range<CGAL::Face_around_target_iterator<Graph>>>(
      module, "Face_around_target_iterator");
  module.def("faces_around_target", &CGAL::faces_around_target<Graph>);
}

}  // namespace pyCGAL
