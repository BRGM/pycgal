#pragma once

#include <CGAL/Plane_3.h>
#include <CGAL/Point_3.h>
#include <CGAL/Vector_3.h>
#include <pyCGAL/typedefs.h>
#include <pybind11/operators.h>

namespace pyCGAL {

template <typename Kernel>
typename WrapTraits<CGAL::Plane_3<Kernel>>::py_class wrap_class(
    WrapTraits<CGAL::Plane_3<Kernel>>, py::module& module) {
  using Wrap = WrapTraits<CGAL::Plane_3<Kernel>>;
  using Plane = typename Wrap::cpp_type;

  typename Wrap::py_class pyclass = py::class_<Plane>(module, "Plane_3");

  using Point = CGAL::Point_3<Kernel>;
  using Vector = CGAL::Vector_3<Kernel>;

  pyclass.def(py::init<const Point&, const Point&, const Point&>());
  pyclass.def(py::init<const Point&, const Vector&>());
  pyclass.def(py::self == py::self);
  pyclass.def(py::self != py::self);
  pyclass.def("projection", &Plane::projection);
  pyclass.def("opposite", &Plane::opposite);
  pyclass.def("point", &Plane::point);
  pyclass.def("orthogonal_vector", &Plane::orthogonal_vector);
  pyclass.def("base1", &Plane::base1);
  pyclass.def("base2", &Plane::base2);
  pyclass.def("to_2d", &Plane::to_2d);
  pyclass.def("to_3d", &Plane::to_3d);
  pyclass.def("has_on",
              py::overload_cast<const Point&>(&Plane::has_on, py::const_));
  pyclass.def("has_on_positive_side", &Plane::has_on_positive_side);
  pyclass.def("has_on_negative_side", &Plane::has_on_negative_side);
  return pyclass;
}

}  // namespace pyCGAL
