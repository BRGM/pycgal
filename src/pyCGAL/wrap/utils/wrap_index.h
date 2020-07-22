#pragma once

namespace pyCGAL {
namespace wrap {
namespace utils {

template <typename Index, typename Base_index>
void wrap_index(py::module& module, const std::string& name) {
  py::class_<Index>(module, name.c_str())
      .def("__str__", [name](const Index& self) {
        std::ostringstream os;
        os << name << "(" << static_cast<Base_index>(self) << ")";
        return os.str();
      });
}

}  // namespace utils
}  // namespace wrap
}  // namespace pyCGAL
