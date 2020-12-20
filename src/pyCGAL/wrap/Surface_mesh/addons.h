#pragma once

#include <CGAL/Kernel_traits.h>
#include <CGAL/Plane_3.h>

#include "detail/addons.h"

namespace pyCGAL {

namespace detail = pyCGAL::wrap::Surface_mesh::detail;

template <typename Surface_mesh>
void wrap_element(detail::addons<Surface_mesh>, py::module& module) {
  using Point = typename Surface_mesh::Point;
  using Kernel = typename CGAL::Kernel_traits<Point>::Kernel;
  using Plane = CGAL::Plane_3<Kernel>;

  module.def("project", [](Surface_mesh& mesh, Plane& plane) {
    for (auto&& v : mesh.vertices()) {
      Point& P = mesh.point(v);
      P = plane.projection(P);
    }
  });
}

}  // namespace pyCGAL
