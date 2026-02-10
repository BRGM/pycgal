#pragma once

#include <limits>

namespace pyCGAL {

namespace PMP {

namespace utils {

template <typename Mesh>
auto smallest_edge(const Mesh& mesh) {
  double l2 = std::numeric_limits<double>::max();
  auto emin = Mesh::null_edge();
  for (auto&& e : mesh.edges()) {
    auto d2 = CGAL::squared_distance(mesh.point(mesh.vertex(e, 0)),
                                     mesh.point(mesh.vertex(e, 1)));
    if (d2 < l2) {
      l2 = d2;
      emin = e;
    }
  }
  return std::make_pair(emin, l2);
}

}  // namespace utils

}  // namespace PMP

}  // namespace pyCGAL
