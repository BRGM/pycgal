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
  const char* name = nullptr;
};

template <typename EnumType>
struct CGALEnum {
  using enum_type = EnumType;
  using py_enum = py::enum_<EnumType>;
};

template <typename Class>
typename WrapTraits<Class>::py_class wrap_class(WrapTraits<Class>, py::module&);

// A generic utility to wrap algorithms, enums...
template <typename Wrapped>
void wrap_element(Wrapped, py::module&);

}  // namespace pyCGAL
