#pragma once

#include <pyCGAL/wrap/Surface_mesh/Property_capsule.h>
#include <pybind11/pybind11.h>

#include <optional>
#include <type_traits>

namespace pyCGAL::wrap::utils {

namespace py = pybind11;

template <typename Index, typename T, typename Mesh>
auto convert_to_property_map(py::object pmap, Mesh& mesh, T default_value) {
  // It's ok to use std::optional as return types as Property_maps are actually
  // pointers
  using Property_map = typename Mesh::template Property_map<Index, T>;
  using Result = std::optional<Property_map>;
  if (pmap.is_none()) return Result{};
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
    return Result{result.first};
  }
  if (py::hasattr(pmap, "property_capsule")) {
    using Property_capsule =
        pyCGAL::wrap::Surface_mesh::Property_capsule<Mesh, Index>;
    Property_map* p = nullptr;
    try {
      auto capsule = pmap.attr("property_capsule")().cast<Property_capsule>();
      p = capsule.template get_map_if<T>();
    } catch (py::cast_error) {
    }
    if (!p)
      throw std::runtime_error(
          "Could not convert python object to an actual property map.");
    return Result{*p};
  }
  return Result{};
}

template <typename Index, typename Mesh>
auto convert_to_property_flag(py::object pmap, Mesh& mesh,
                              const bool default_value = false) {
  return convert_to_property_map<Index, bool>(pmap, mesh, default_value);
}

}  // namespace pyCGAL::wrap::utils
