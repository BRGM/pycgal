#pragma once

#include <CGAL/Dimension.h>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>

#include "pyCGAL/wrap/utils/CGAL_FT.h"

namespace py = pybind11;

namespace pyCGAL::wrap::utils {

template <typename Vector>
void wrap_vector_operators(py::class_<Vector>& pyVector) {
  pyVector.def("squared_length", &Vector::squared_length);
  pyVector.def(-py::self);
  pyVector.def(py::self * py::self);
  using FT = CGAL_FT<Vector>;
  pyVector.def(py::self *= FT());
  pyVector.def(py::self * FT());
  pyVector.def(FT() * py::self);
}

}  // namespace pyCGAL::wrap::utils
