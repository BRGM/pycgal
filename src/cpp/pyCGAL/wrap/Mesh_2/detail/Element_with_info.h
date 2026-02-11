#pragma once
#include <pyCGAL/typedefs.h>

namespace pyCGAL::wrap::Mesh_2::detail {

template <typename Wrap>
typename Wrap::py_class wrap_element_with_info(py::module& module,
                                               const char* name) {
  using cpp_type = typename Wrap::cpp_type;

  typename Wrap::py_class pyclass = py::class_<cpp_type>(module, name);
  pyclass.def(py::init<>());
  pyclass.def_property(
      "info", py::overload_cast<>(&cpp_type::info),
      [](cpp_type& self, py::object& obj) { self.info() = obj; });
  return pyclass;
}

}  // namespace pyCGAL::wrap::Mesh_2::detail
