#pragma once

#include <pybind11/pybind11.h>
namespace py = pybind11;

#include <pyCGAL/wrap/utils/Coordinates_array.h>

#include "add_vertices.h"
#include "extend_mesh.h"

namespace pyCGAL::wrap::utils {

template <typename Surface_mesh>
auto make_mesh(utils::Coordinates_array<typename Surface_mesh::Point>& vertices,
               py::list faces, Extension_data<Surface_mesh>& data) {
  auto mesh = std::make_unique<Surface_mesh>();
  extend_mesh(*mesh, data, vertices, faces);
  return mesh;
}

template <typename Surface_mesh>
auto make_mesh(utils::Coordinates_array<typename Surface_mesh::Point>& vertices,
               py::list faces) {
  Extension_data<Surface_mesh> data;
  return make_mesh(vertices, faces, data);
}

}  // namespace pyCGAL::wrap::utils
