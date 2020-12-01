#pragma once

#include <CGAL/boost/graph/Euler_operations.h>
#include <pyCGAL/typedefs.h>

#include "detail/Euler_operations.h"

namespace pyCGAL {

template <typename Graph>
void wrap_element(wrap::BGL::detail::Euler_operations<Graph>,
                  py::module& module) {
  module.def("join_vertex", &CGAL::Euler::join_vertex<Graph>);
  module.def("split_vertex", &CGAL::Euler::split_vertex<Graph>);
  module.def("split_edge", &CGAL::Euler::split_edge<Graph>);
  module.def("join_face", &CGAL::Euler::join_face<Graph>);
  module.def("split_face", &CGAL::Euler::split_face<Graph>);
  module.def("join_loop", &CGAL::Euler::join_loop<Graph>);
  module.def("split_loop", &CGAL::Euler::split_loop<Graph>);
  module.def("remove_face", &CGAL::Euler::remove_face<Graph>);
  module.def("add_edge", &CGAL::Euler::add_edge<Graph>);
  // module.def("add_face", &CGAL::Euler::add_face<Graph>); // needs VertexRange
  // to be available
  module.def("make_hole", &CGAL::Euler::make_hole<Graph>);
  module.def("fill_hole", &CGAL::Euler::fill_hole<Graph>);
  module.def("add_center_vertex", &CGAL::Euler::add_center_vertex<Graph>);
  module.def("remove_center_vertex", &CGAL::Euler::remove_center_vertex<Graph>);
  module.def("add_vertex_and_face_to_border",
             &CGAL::Euler::add_vertex_and_face_to_border<Graph>);
  module.def("add_face_to_border", &CGAL::Euler::add_face_to_border<Graph>);
  module.def("collapse_edge", &CGAL::Euler::collapse_edge<Graph>);
  module.def("flip_edge", &CGAL::Euler::flip_edge<Graph>);
  module.def("does_satisfy_link_condition",
             &CGAL::Euler::does_satisfy_link_condition<Graph>);
}

}  // namespace pyCGAL
