#pragma once

#include <CGAL/centroid.h>
#include <pyCGAL/wrap/utils/Coordinates_array.h>
#include <pyCGAL/wrap/utils/generic_array.h>

namespace pyCGAL::wrap::utils {

template <typename Surface_mesh>
inline auto centroid(const Surface_mesh& mesh,
                     const typename Surface_mesh::Face_index f) ->
    typename Surface_mesh::Point {
  using Point = typename Surface_mesh::Point;
  std::vector<Point> vertices;
  for (auto&& v : CGAL::vertices_around_face(mesh.halfedge(f), mesh)) {
    vertices.emplace_back(mesh.point(v));
  }
  return CGAL::centroid(begin(vertices), end(vertices));
}

template <typename Surface_mesh>
auto centroids(const Surface_mesh& mesh)
    -> Output_coordinate_array<typename Surface_mesh::Point> {
  using Point = typename Surface_mesh::Point;
  auto result = empty_coordinates_array<Point>(mesh.number_of_faces());
  constexpr int dim = CGAL::Ambient_dimension<Point>::value;
  auto centers = make_array_of_array<CGAL_FT<Point>, dim>(result);
  auto p = begin(centers);
  for (auto&& f : mesh.faces()) {
    *(reinterpret_cast<Point*>(p)) = centroid(mesh, f);
    ++p;
  }
  assert(p == end(centers));
  return result;
}

}  // namespace pyCGAL::wrap::utils
