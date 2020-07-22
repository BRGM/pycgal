#pragma once

#include <pybind11/numpy.h>

namespace py = pybind11;

namespace pyCGAL {
namespace wrap {
namespace utils {

template <typename Range>
auto wrap_range(py::module& module, const std::string& name) {
  py::class_<Range>(module, name.c_str())
      .def("__iter__", [](const Range& self) {
        return py::make_iterator(begin(self), end(self));
      });
}

}  // namespace utils
}  // namespace wrap
}  // namespace pyCGAL
