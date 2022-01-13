#pragma once

#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <pyCGAL/typedefs.h>

#include "utils/as_arrays.h"

namespace pyCGAL {
template <typename Traits, typename Tds, typename Itag>
typename WrapTraits<
    CGAL::Constrained_Delaunay_triangulation_2<Traits, Tds, Itag>>::py_class
wrap_class(
    WrapTraits<CGAL::Constrained_Delaunay_triangulation_2<Traits, Tds, Itag>>
        wrap,
    py::module& module) {
  using Wrap =
      WrapTraits<CGAL::Constrained_Delaunay_triangulation_2<Traits, Tds, Itag>>;
  using CDt = typename Wrap::cpp_type;

  using Vertex_handle = typename CDt::Vertex_handle;
  using Face_handle = typename CDt::Face_handle;
  using Point = typename CDt::Point;

  // using Ct = CGAL::Constrained_triangulation_2<Traits, Tds, Itag>;
  // static_assert(std::is_base_of_v<Ct, CDt>);

  // typename Wrap::py_class pyclass = py::class_<CDt, Ct>(module, wrap.name);
  typename Wrap::py_class pyclass = py::class_<CDt>(module, wrap.name);
  pyclass.def(py::init<>());
  pyclass.def(py::init<const CDt&>());
  // Triangulation_2 members
  pyclass.def("clear", &CDt::clear);
  pyclass.def("dimension", &CDt::dimension);
  pyclass.def("number_of_vertices", &CDt::number_of_vertices);
  pyclass.def("number_of_faces", &CDt::number_of_faces);
  // Constrained_triangulation_2 members
  pyclass.def("insert",
              [](CDt& self, const Point& P) { return self.insert(P); });
  pyclass.def("insert_constraint",
              [](CDt& self, const Point& P, const Point& Q) {
                return self.insert_constraint(P, Q);
              });
  pyclass.def("insert_constraint",
              [](CDt& self, Vertex_handle vP, Vertex_handle vQ) {
                return self.insert_constraint(vP, vQ);
              });
  // Utilities
  pyclass.def("as_arrays",
              [](const CDt& self) { return wrap::utils::as_arrays(self); });
  module.def("as_arrays", [](const CDt& triangulation) {
    return wrap::utils::as_arrays(triangulation);
  });

  return pyclass;
}

}  // namespace pyCGAL
