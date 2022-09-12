#pragma once

#include <CGAL/boost/graph/iterator.h>
#include <pyCGAL/typedefs.h>
#include <pyCGAL/wrap/core/Iterator_range.h>

#include "detail/iterators.h"

namespace pyCGAL {

template <typename Graph>
void wrap_element(wrap::BGL::detail::iterators<Graph>, py::module& module) {
  using halfedge_descriptor =
      typename boost::graph_traits<Graph>::halfedge_descriptor;
  using vertex_descriptor =
      typename boost::graph_traits<Graph>::vertex_descriptor;

  wrap_class(
      WrapTraits<
          CGAL::Iterator_range<CGAL::Halfedge_around_source_iterator<Graph>>>{
          "Halfedge_around_source_iterator"},
      module);
  module.def("halfedges_around_source",
             py::overload_cast<halfedge_descriptor, const Graph&>(
                 &CGAL::halfedges_around_source<Graph>));
  module.def("halfedges_around_source",
             py::overload_cast<vertex_descriptor, const Graph&>(
                 &CGAL::halfedges_around_source<Graph>));

  wrap_class(
      WrapTraits<
          CGAL::Iterator_range<CGAL::Halfedge_around_target_iterator<Graph>>>{
          "Halfedge_around_target_iterator"},
      module);
  module.def("halfedges_around_target",
             py::overload_cast<halfedge_descriptor, const Graph&>(
                 &CGAL::halfedges_around_target<Graph>));
  module.def("halfedges_around_target",
             py::overload_cast<vertex_descriptor, const Graph&>(
                 &CGAL::halfedges_around_target<Graph>));

  wrap_class(
      WrapTraits<
          CGAL::Iterator_range<CGAL::Halfedge_around_face_iterator<Graph>>>{
          "Halfedge_around_face_iterator"},
      module);
  module.def("halfedges_around_face", &CGAL::halfedges_around_face<Graph>);

  wrap_class(
      WrapTraits<
          CGAL::Iterator_range<CGAL::Face_around_target_iterator<Graph>>>{
          "Face_around_target_iterator"},
      module);
  module.def("faces_around_target", &CGAL::faces_around_target<Graph>);

  wrap_class(
      WrapTraits<CGAL::Iterator_range<CGAL::Face_around_face_iterator<Graph>>>{
          "Face_around_face_iterator"},
      module);
  module.def("faces_around_face", &CGAL::faces_around_face<Graph>);

  wrap_class(
      WrapTraits<
          CGAL::Iterator_range<CGAL::Vertex_around_target_iterator<Graph>>>{
          "Vertex_around_target_iterator"},
      module);
  module.def("vertices_around_target",
             py::overload_cast<vertex_descriptor, const Graph&>(
                 &CGAL::vertices_around_target<Graph>));
  module.def("vertices_around_target",
             py::overload_cast<vertex_descriptor, const Graph&>(
                 &CGAL::vertices_around_target<Graph>));

  wrap_class(
      WrapTraits<
          CGAL::Iterator_range<CGAL::Vertex_around_face_iterator<Graph>>>{
          "Vertex_around_face_iterator"},
      module);
  module.def("vertices_around_face", &CGAL::vertices_around_face<Graph>);
}

}  // namespace pyCGAL
