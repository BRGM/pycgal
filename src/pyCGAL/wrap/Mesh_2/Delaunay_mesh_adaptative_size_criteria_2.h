#pragma once

#include <pyCGAL/addons/Mesh_2/Delaunay_mesh_adaptative_size_criteria_2.h>
#include <pyCGAL/typedefs.h>

namespace pyCGAL {

template <typename ConstrainedDelaunayTriangulation>
typename WrapTraits<addons::Delaunay_mesh_adaptative_size_criteria_2<
    ConstrainedDelaunayTriangulation>>::py_class
wrap_class(WrapTraits<addons::Delaunay_mesh_adaptative_size_criteria_2<
               ConstrainedDelaunayTriangulation>>,
           py::module& module) {
  using Wrap = WrapTraits<addons::Delaunay_mesh_adaptative_size_criteria_2<
      ConstrainedDelaunayTriangulation>>;
  using Criteria = typename Wrap::cpp_type;
  using Point = typename ConstrainedDelaunayTriangulation::Point;

  typename Wrap::py_class pyclass =
      py::class_<Criteria>(module, "Delaunay_mesh_adaptative_size_criteria_2");
  pyclass.def(py::init([](const double& b, const double& S) {
                return Criteria{[S](const Point&) { return S; }, b};
              }),
              py::arg("b") = 0.125, py::arg("S") = 0);
  pyclass.def(py::init([](py::function sizing_field, const double& b) {
                return Criteria{[sizing_field](const Point& P) {
                                  py::object s = sizing_field(P);
                                  return s.cast<double>();
                                },
                                b};
              }),
              py::arg("sizing_field"), py::arg("b") = 0.125);

  return pyclass;
}

}  // namespace pyCGAL
