#pragma once

#include "../Coordinates_array.h"
#include "../generic_array.h"

namespace pyCGAL {
namespace wrap {
namespace utils {
namespace SM {

template <typename Surface_mesh>
auto centroids(const Surface_mesh& mesh)
    -> Output_coordinate_array<typename Surface_mesh::Point> {
  using Point = typename Surface_mesh::Point;
  auto result = empty_coordinates_array<Point>(mesh.number_of_faces());
  constexpr int dim = CGAL::Ambient_dimension<Point>::value;
  auto centers = make_array_of_array<CGAL_FT<Point>, dim>(result);
  auto p = begin(centers);
  for (auto&& f : mesh.faces()) {
    auto& C = *p;
    C.fill(0);
    CGAL_FT<Point> nv = 0;
    for (auto&& v : CGAL::vertices_around_face(mesh.halfedge(f), mesh)) {
      assert(v != mesh.null_vertex());
      const Point& P = mesh.point(v);
      for (int i = 0; i < dim; ++i) C[i] += P[i];
      ++nv;
    }
    if (nv > 0) {
      const auto invnv = 1 / nv;
      for (int i = 0; i < dim; ++i) C[i] *= invnv;
    }
    ++p;
  }
  assert(p == end(centers));
  return result;
}

}  // namespace SM
}  // namespace utils
}  // namespace wrap
}  // namespace pyCGAL
