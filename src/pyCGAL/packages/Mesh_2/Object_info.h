#pragma once

#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace pyCGAL::Mesh_2 {

// A thin wrapper around py::object that default constructs to PyNone
struct Object_info : py::object {
  using py::object::object;
  Object_info() : py::object{py::none{}} {}
  Object_info(const py::object& other) : py::object{other} {}
  Object_info(py::object&& other)
      : py::object{std::forward<py::object>(other)} {}
};

}  // namespace pyCGAL::Mesh_2
