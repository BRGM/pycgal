#pragma once

#include <pybind11/pybind11.h>
namespace py = pybind11;

namespace pyCGAL {

template <typename CGALClass>
struct Wrapper {
  using cgal_type = CGALClass;
  using pybind_class = py::class_<CGALClass>;
  static pybind_class wrap(py::module&);
};

}  // namespace pyCGAL
