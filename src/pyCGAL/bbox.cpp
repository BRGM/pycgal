#include <CGAL/Bbox_3.h>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
namespace py = pybind11;

PYBIND11_MODULE(bbox, module) {
  using Bbox_3 = CGAL::Bbox_3;
  py::class_<Bbox_3>(module, "Bbox_3")
      .def(py::init<>())
      .def(py::init<double, double, double, double, double, double>())
      .def("dimension", &Bbox_3::dimension)
      .def("xmin", &Bbox_3::xmin)
      .def("ymin", &Bbox_3::ymin)
      .def("zmin", &Bbox_3::zmin)
      .def("xmax", &Bbox_3::xmax)
      .def("ymax", &Bbox_3::ymax)
      .def("zmax", &Bbox_3::zmax)
      .def(py::self == py::self)
      .def(py::self != py::self)
      .def(py::self + py::self)
      .def(py::self += py::self)
      .def("dilate", &Bbox_3::dilate);

  module.def("do_overlap", py::overload_cast<const Bbox_3&, const Bbox_3&>(
                               &CGAL::do_overlap));
}
