#pragma once

#include <CGAL/Triangulation_2.h>
#include <pyCGAL/typedefs.h>

#include "utils/as_arrays.h"

namespace pyCGAL {
template <typename Traits, typename Tds>
typename WrapTraits<CGAL::Triangulation_2<Traits, Tds>>::py_class wrap_class(
    WrapTraits<CGAL::Triangulation_2<Traits, Tds>> wrap, py::module& module) {
  using Wrap = WrapTraits<CGAL::Triangulation_2<Traits, Tds>>;
  using Triangulation = typename Wrap::cpp_type;

  using Vertex_handle = typename Triangulation::Vertex_handle;
  using Face_handle = typename Triangulation::Face_handle;

  py::class_<Vertex_handle>(module, "Vertex_handle");

  py::class_<Face_handle>(module, "Face_handle");

  typename Wrap::py_class pyclass =
      py::class_<Triangulation>(module, wrap.name);
  pyclass.def(py::init<>());
  pyclass.def(py::init<const Triangulation&>());
  pyclass.def("clear", &Triangulation::clear);
  pyclass.def("dimension", &Triangulation::dimension);
  pyclass.def("number_of_vertices", &Triangulation::number_of_vertices);
  pyclass.def("number_of_faces", &Triangulation::number_of_faces);
  pyclass.def("as_arrays", [](const Triangulation& self) {
    return wrap::utils::as_arrays(self);
  });
  module.def("as_arrays", [](const Triangulation& triangulation) {
    return wrap::utils::as_arrays(triangulation);
  });

  return pyclass;
}

}  // namespace pyCGAL
