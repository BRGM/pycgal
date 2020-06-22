#pragma once

#include <CGAL/Dimension.h>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>

#include "CGAL_FT.h"

namespace py = pybind11;

namespace pyCGAL {

namespace wrap {

namespace utils {

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

}  // namespace utils

}  // namespace wrap

}  // namespace pyCGAL
