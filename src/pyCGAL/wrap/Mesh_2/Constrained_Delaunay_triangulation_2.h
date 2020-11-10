#pragma once

#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <pyCGAL/typedefs.h>

#include "utils/as_arrays.h"

namespace pyCGAL {
template <typename Traits, typename Tds, typename Itag>
typename WrapTraits<
    CGAL::Constrained_Delaunay_triangulation_2<Traits, Tds, Itag>>::py_class
wrap_class(
    WrapTraits<CGAL::Constrained_Delaunay_triangulation_2<Traits, Tds, Itag>>,
    py::module& module) {
  using Wrap =
      WrapTraits<CGAL::Constrained_Delaunay_triangulation_2<Traits, Tds, Itag>>;
  using CDT = typename Wrap::cpp_type;

  using Vertex_handle = typename CDT::Vertex_handle;
  using Face_handle = typename CDT::Face_handle;
  using Point = typename CDT::Point;

  py::class_<Vertex_handle>(module, "Vertex_handle");

  py::class_<Face_handle>(module, "Face_handle");

  typename Wrap::py_class pyclass =
      py::class_<CDT>(module, "Constrained_Delaunay_triangulation_2");
  pyclass.def(py::init<>());
  pyclass.def("number_of_vertices", &CDT::number_of_vertices);
  pyclass.def("number_of_faces", &CDT::number_of_faces);
  pyclass.def("insert",
              [](CDT& self, const Point& P) { return self.insert(P); });
  pyclass.def("insert_constraint",
              [](CDT& self, const Point& P, const Point& Q) {
                return self.insert_constraint(P, Q);
              });
  pyclass.def("insert_constraint",
              [](CDT& self, Vertex_handle vP, Vertex_handle vQ) {
                return self.insert_constraint(vP, vQ);
              });
  pyclass.def("as_arrays",
              [](const CDT& self) { return wrap::utils::as_arrays(self); });

  module.def("as_arrays",
             [](const CDT& cdt) { return wrap::utils::as_arrays(cdt); });

  return pyclass;
}

}  // namespace pyCGAL
