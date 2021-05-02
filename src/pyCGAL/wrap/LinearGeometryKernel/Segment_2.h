#pragma once

#include <CGAL/Line_2.h>
#include <CGAL/Point_2.h>
#include <CGAL/Segment_2.h>
#include <CGAL/Vector_2.h>
#include <pyCGAL/typedefs.h>
#include <pybind11/operators.h>

namespace pyCGAL {

template <typename Kernel>
typename WrapTraits<CGAL::Segment_2<Kernel>>::py_class wrap_class(
    WrapTraits<CGAL::Segment_2<Kernel>>, py::module& module) {
  using Wrap = WrapTraits<CGAL::Segment_2<Kernel>>;
  using Segment = typename Wrap::cpp_type;

  typename Wrap::py_class pyclass = py::class_<Segment>(module, "Segment_2");

  using Point = CGAL::Point_2<Kernel>;
  using Line = CGAL::Line_2<Kernel>;
  using Vector = CGAL::Vector_2<Kernel>;

  pyclass.def(py::init<const Segment&>());
  pyclass.def(py::init<const Point&, const Point&>());
  pyclass.def(py::self == py::self);
  pyclass.def(py::self != py::self);
  pyclass.def("source", &Segment::source);
  pyclass.def("target", &Segment::target);
  pyclass.def("min", &Segment::min);
  pyclass.def("max", &Segment::max);
  pyclass.def("vertex", &Segment::vertex);
  pyclass.def("__getitem__", &Segment::operator[]);
  pyclass.def("squared_length", &Segment::squared_length);
  pyclass.def("to_vector", &Segment::to_vector);
  pyclass.def("opposite", &Segment::opposite);
  pyclass.def("supporting_line", &Segment::supporting_line);
  pyclass.def("is_degenerate", &Segment::is_degenerate);
  pyclass.def("has_on",
              py::overload_cast<const Point&>(&Segment::has_on, py::const_));
  pyclass.def("bbox", &Segment::bbox);
  return pyclass;
}

}  // namespace pyCGAL
