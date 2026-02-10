#pragma once

#include <pyCGAL/wrap/utils/Coordinates_array.h>
#include <pyCGAL/wrap/utils/generic_array.h>

#include "Vertex_map.h"

namespace pyCGAL::wrap::utils {

template <typename Surface_mesh>
void add_vertices(Surface_mesh& mesh,
                  Coordinates_array<typename Surface_mesh::Point>& vertices,
                  Vertex_map<Surface_mesh>& vmap) {
  typedef typename Surface_mesh::Point Point;
  auto va = make_point_array<Point>(vertices);
  vmap.reserve(vmap.size() + va.size());
  for (auto&& P : va) {
    const auto v = mesh.add_vertex(P);
    if (v == Surface_mesh::null_vertex())
      throw std::runtime_error("Could not create vertex!");
    vmap.emplace_back(v);
  }
}

}  // namespace pyCGAL::wrap::utils
