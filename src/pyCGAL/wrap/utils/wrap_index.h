#pragma once

#include <pybind11/operators.h>

namespace pyCGAL {
namespace wrap {
namespace utils {

namespace local {
template <typename Base_index>
std::string index_representation(const std::string& name, const Base_index i) {
  std::ostringstream os;
  os << name << "(" << i << ")";
  return os.str();
}
}  // namespace local

template <typename Index, typename Base_index>
void wrap_index(py::module& module, const std::string& name) {
  py::class_<Index> pyclass = py::class_<Index>{module, name.c_str()};
  pyclass.def(py::init<const Index&>());
  pyclass.def_static(
      "create", [](const Base_index i) { return std::make_unique<Index>(i); });
  pyclass.def("__str__", [name](const Index& self) {
    return local::index_representation(name, static_cast<Base_index>(self));
  });
  pyclass.def("__repr__", [name](const Index& self) {
    return local::index_representation(name, static_cast<Base_index>(self));
  });
  pyclass.def(py::hash(py::self));
  pyclass.def(py::self == py::self);
  pyclass.def(py::self != py::self);
}

}  // namespace utils
}  // namespace wrap
}  // namespace pyCGAL
