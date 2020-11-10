#pragma once

#include <pybind11/pybind11.h>
namespace py = pybind11;

#include <pyCGAL/wrap/utils/Coordinates_array.h>

#include "extend_mesh.h"

namespace pyCGAL::wrap::utils {

template <typename Surface_mesh>
auto make_mesh(utils::Coordinates_array<typename Surface_mesh::Point>& vertices,
               py::list faces) -> std::unique_ptr<Surface_mesh> {
  auto mesh = std::make_unique<Surface_mesh>();
  extend_mesh(*mesh, vertices, faces);
  return mesh;
}

}  // namespace pyCGAL::wrap::utils
