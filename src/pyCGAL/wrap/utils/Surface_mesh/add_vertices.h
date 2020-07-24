#pragma once

#include "../Coordinates_array.h"
#include "../generic_array.h"
#include "Vertex_map.h"

namespace pyCGAL {
namespace wrap {
namespace utils {
namespace SM {

template <typename Surface_mesh>
auto add_vertices(Surface_mesh& mesh,
                  Coordinates_array<typename Surface_mesh::Point>& vertices) {
  typedef typename Surface_mesh::Point Point;
  auto va = make_point_array<Point>(vertices);
  Vertex_map<Surface_mesh> vmap;
  vmap.reserve(va.size());
  for (auto&& P : va) {
    const auto v = mesh.add_vertex(P);
    if (v == Surface_mesh::null_vertex())
      throw std::runtime_error("Could not create vertex!");
    vmap.emplace_back(v);
  }
  return vmap;
}

}  // namespace SM
}  // namespace utils
}  // namespace wrap
}  // namespace pyCGAL
