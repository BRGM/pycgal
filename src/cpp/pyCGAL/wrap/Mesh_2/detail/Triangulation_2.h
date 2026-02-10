#pragma once

#include <CGAL/Triangulation_2.h>
#include <pyCGAL/typedefs.h>
#include <pyCGAL/wrap/core/Iterator_range.h>

#include "../utils/as_arrays.h"

namespace pyCGAL::wrap::Mesh_2::detail {

template <typename Triangulation, typename Py_class>
Py_class& wrap_Triangulation_2(Py_class& pyclass) {
  pyclass.def("clear", &Triangulation::clear);
  pyclass.def("dimension", &Triangulation::dimension);
  pyclass.def("number_of_vertices", &Triangulation::number_of_vertices);
  pyclass.def("number_of_faces", &Triangulation::number_of_faces);
  pyclass.def("points", &Triangulation::points);
  pyclass.def("as_arrays", &wrap::utils::as_arrays<Triangulation>);
  return pyclass;
}

}  // namespace pyCGAL::wrap::Mesh_2::detail
