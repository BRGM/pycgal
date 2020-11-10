#pragma once

#include <CGAL/Bbox_3.h>
#include <pyCGAL/typedefs.h>
#include <pybind11/operators.h>

namespace pyCGAL {

template <>
typename WrapTraits<CGAL::Bbox_3>::py_class wrap_class(WrapTraits<CGAL::Bbox_3>,
                                                       py::module& module) {
  using Wrap = WrapTraits<CGAL::Bbox_3>;
  using Bbox_3 = typename Wrap::cpp_type;

  typename Wrap::py_class pyclass = py::class_<Bbox_3>(module, "Bbox_3");
  pyclass.def(py::init<>());
  pyclass.def(py::init<double, double, double, double, double, double>());
  pyclass.def("dimension", &Bbox_3::dimension);
  pyclass.def("xmin", &Bbox_3::xmin);
  pyclass.def("ymin", &Bbox_3::ymin);
  pyclass.def("zmin", &Bbox_3::zmin);
  pyclass.def("xmax", &Bbox_3::xmax);
  pyclass.def("ymax", &Bbox_3::ymax);
  pyclass.def("zmax", &Bbox_3::zmax);
  pyclass.def(py::self == py::self);
  pyclass.def(py::self != py::self);
  pyclass.def(py::self + py::self);
  pyclass.def(py::self += py::self);
  pyclass.def("dilate", &Bbox_3::dilate);

  module.def("do_overlap", py::overload_cast<const Bbox_3&, const Bbox_3&>(
                               &CGAL::do_overlap));

  return pyclass;
}

}  // namespace pyCGAL
