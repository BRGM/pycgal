#pragma once

#include <CGAL/boost/graph/iterator.h>
#include <pyCGAL/typedefs.h>
#include <pyCGAL/wrap/core/Iterator_range.h>

#include "detail/iterators.h"

namespace pyCGAL {

template <typename Graph>
void wrap_element(wrap::BGL::detail::iterators<Graph>, py::module& module) {
  wrap_class(
      WrapTraits<
          CGAL::Iterator_range<CGAL::Halfedge_around_face_iterator<Graph>>>{
          "Halfedge_around_face_iterator"},
      module);
  module.def("halfedges_around_face", &CGAL::halfedges_around_face<Graph>);

  wrap_class(
      WrapTraits<
          CGAL::Iterator_range<CGAL::Vertex_around_face_iterator<Graph>>>{
          "Vertex_around_face_iterator"},
      module);
  module.def("vertices_around_face", &CGAL::vertices_around_face<Graph>);
  module.def("vertices_around_face",
             [](const typename Graph::Face_index& face, const Graph& graph) {
               return CGAL::vertices_around_face(graph.halfedge(face), graph);
             });

  wrap_class(
      WrapTraits<
          CGAL::Iterator_range<CGAL::Face_around_target_iterator<Graph>>>{
          "Face_around_target_iterator"},
      module);
  module.def("faces_around_target", &CGAL::faces_around_target<Graph>);
}

}  // namespace pyCGAL
