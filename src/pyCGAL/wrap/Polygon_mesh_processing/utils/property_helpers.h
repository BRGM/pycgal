#pragma once

#include <pyCGAL/wrap/Surface_mesh/Property_capsule.h>
#include <pybind11/pybind11.h>

#include <type_traits>

namespace pyCGAL::wrap::utils {

namespace py = pybind11;

template <typename Index, typename Mesh, typename T>
auto convert_to_property_map(py::object pmap, Mesh& mesh, T default_value)
    -> std::add_pointer_t<typename Mesh::template Property_map<Index, T>> {
  using Property_map = typename Mesh::template Property_map<Index, T>;
  if (py::isinstance<py::str>(pmap)) {
    py::str name = pmap;
    auto result = mesh.template add_property_map<Index>(name, default_value);
#ifndef NDEBUG
    if (result.second) {
      py::print("Creating property map:", name);
    } else {
      py::print("Retrieving property map:", name);
    }
#endif
    return &result.first;
  }
  Property_map* p = nullptr;
  try {
    if (py::hasattr(pmap, "property_capsule")) {
      using Property_capsule =
          pyCGAL::wrap::Surface_mesh::Property_capsule<Mesh, Index>;
      auto capsule = pmap.attr("property_capsule")().cast<Property_capsule>();
      p = capsule.template get_map_if<T>();
    }
  } catch (py::cast_error) {
    assert(!p);
  }
  if (!p) {
    throw std::runtime_error(
        "Could not convert python object to an actual property map.");
  }
  return p;
}

template <typename Index, typename Mesh>
auto convert_to_property_flag(py::object pmap, Mesh& mesh,
                              const bool default_value = false)
    -> std::add_pointer_t<typename Mesh::template Property_map<Index, bool>> {
  return convert_to_property_map<Index, Mesh, bool>(pmap, mesh, default_value);
}

}  // namespace pyCGAL::wrap::utils
