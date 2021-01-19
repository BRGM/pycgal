#pragma once

#include <CGAL/Polygon_mesh_processing/clip.h>
#include <pyCGAL/typedefs.h>

#include <stdexcept>

#include "detail/split.h"

namespace pyCGAL {

namespace detail = pyCGAL::wrap::Polygon_mesh_processing::detail;

template <typename TriangleMesh, typename Plane_3>
void wrap_element(detail::split<TriangleMesh, Plane_3>, py::module& module) {
  module.def(
      "split",
      [](TriangleMesh& tm, TriangleMesh& splitter) {
        if (!CGAL::is_triangle_mesh(tm) || !CGAL::is_triangle_mesh(splitter))
          throw std::runtime_error("Only triangle meshes can be splitted!");
        CGAL::Polygon_mesh_processing::split(tm, splitter);
      },
      py::arg("tm").none(false), py::arg("splitter").none(false));
  module.def(
      "split",
      [](TriangleMesh& tm, const Plane_3& plane,
         const bool throw_on_self_intersection,
         const bool allow_self_intersections) {
        if (!CGAL::is_triangle_mesh(tm))
          throw std::runtime_error("Only triangle meshes can be splitted!");

        namespace pns = CGAL::Polygon_mesh_processing::parameters;

        CGAL::Polygon_mesh_processing::split(
            tm, plane,
            pns::throw_on_self_intersection(throw_on_self_intersection)
                .clip_volume(allow_self_intersections));
      },
      py::arg("tm").none(false), py::arg("plane").none(false),
      py::arg("throw_on_self_intersection") = false,
      py::arg("allow_self_intersections") = false);
}

}  // namespace pyCGAL
