#pragma once

#include <CGAL/Line_3.h>
#include <CGAL/Point_3.h>
#include <CGAL/Segment_3.h>
#include <CGAL/Vector_3.h>
#include <pyCGAL/typedefs.h>
#include <pybind11/operators.h>

namespace pyCGAL {

template <typename Kernel>
typename WrapTraits<CGAL::Line_3<Kernel>>::py_class wrap_class(
    WrapTraits<CGAL::Line_3<Kernel>>, py::module& module) {
  using Wrap = WrapTraits<CGAL::Line_3<Kernel>>;
  using Line = typename Wrap::cpp_type;

  typename Wrap::py_class pyclass = py::class_<Line>(module, "Line_3");

  using Point = CGAL::Point_3<Kernel>;
  using Segment = CGAL::Segment_3<Kernel>;
  using Vector = CGAL::Vector_3<Kernel>;

  pyclass.def(py::init<const Line&>());
  pyclass.def(py::init<const Point&, const Point&>());
  pyclass.def(py::init<const Point&, const Vector&>());
  pyclass.def(py::init<const Segment&>());
  pyclass.def(py::self == py::self);
  pyclass.def(py::self != py::self);
  pyclass.def("projection", &Line::projection);
  pyclass.def("point", py::overload_cast<>(&Line::point, py::const_));
  pyclass.def("point", py::overload_cast<const typename Kernel::FT>(
                           &Line::point, py::const_));
  pyclass.def("is_degenerate", &Line::is_degenerate);
  pyclass.def("has_on",
              py::overload_cast<const Point&>(&Line::has_on, py::const_));
  pyclass.def("perpendicular_plane", &Line::perpendicular_plane);
  pyclass.def("opposite", &Line::opposite);
  pyclass.def("to_vector", &Line::to_vector);
  return pyclass;
}

}  // namespace pyCGAL
