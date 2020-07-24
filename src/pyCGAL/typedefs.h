#pragma once

#include <pybind11/pybind11.h>
namespace py = pybind11;

namespace pyCGAL {

// Can be specialized to add usefull members or methods
// such as name of the wrap class...
template <typename Class>
struct WrapTraits {
  using cpp_type = Class;
  using py_class = py::class_<cpp_type>;
};

template <typename Class>
typename WrapTraits<Class>::py_class wrap_class(WrapTraits<Class>, py::module&);

template <typename Algorithm>
void wrap_algorithm(Algorithm, py::module&);

}  // namespace pyCGAL
