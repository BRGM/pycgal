#pragma once

#include <CGAL/Line_3.h>
#include <CGAL/Point_3.h>
#include <CGAL/Ray_3.h>
#include <CGAL/Segment_3.h>
#include <CGAL/Vector_3.h>
#include <pyCGAL/typedefs.h>
#include <pybind11/operators.h>

namespace pyCGAL {

template <typename Kernel>
typename WrapTraits<CGAL::Ray_3<Kernel>>::py_class wrap_class(
    WrapTraits<CGAL::Ray_3<Kernel>>, py::module& module) {
  using Wrap = WrapTraits<CGAL::Ray_3<Kernel>>;
  using Ray = typename Wrap::cpp_type;

  typename Wrap::py_class pyclass = py::class_<Ray>(module, "Ray_3");

  using Line = CGAL::Line_3<Kernel>;
  using Point = CGAL::Point_3<Kernel>;
  using Segment = CGAL::Segment_3<Kernel>;
  using Vector = CGAL::Vector_3<Kernel>;

  pyclass.def(py::init<const Ray&>());
  pyclass.def(py::init<const Point&, const Point&>());
  pyclass.def(py::init<const Point&, const Vector&>());
  pyclass.def(py::init<const Point&, const Line&>());
  pyclass.def(py::self == py::self);
  pyclass.def(py::self != py::self);
  pyclass.def("source", &Ray::source);
  pyclass.def("point", py::overload_cast<const typename Kernel::FT>(
                           &Ray::point, py::const_));
  pyclass.def("to_vector", &Ray::to_vector);
  pyclass.def("supporting_line", &Ray::supporting_line);
  pyclass.def("opposite", &Ray::opposite);
  pyclass.def("is_degenerate", &Ray::is_degenerate);
  pyclass.def("has_on",
              py::overload_cast<const Point&>(&Ray::has_on, py::const_));
  return pyclass;
}

}  // namespace pyCGAL
