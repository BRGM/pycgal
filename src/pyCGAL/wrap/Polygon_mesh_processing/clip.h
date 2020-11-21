#pragma once

#include <CGAL/Polygon_mesh_processing/clip.h>
#include <pyCGAL/typedefs.h>

#include <stdexcept>

#include "detail/clip.h"

namespace pyCGAL {

namespace detail = pyCGAL::wrap::Polygon_mesh_processing::detail;

template <typename TriangleMesh, typename Plane_3>
void wrap_element(detail::clip<TriangleMesh, Plane_3>, py::module& module) {
  module.def(
      "clip",
      [](TriangleMesh& mesh, const Plane_3& plane,
         const bool throw_on_self_intersection, const bool clip_volume,
         const bool use_compact_clipper) {
        if (!CGAL::is_triangle_mesh(mesh))
          throw std::runtime_error("Only triangle meshes can be clipped!");

        namespace pns = CGAL::Polygon_mesh_processing::parameters;

        return CGAL::Polygon_mesh_processing::clip(
            mesh, plane,
            pns::throw_on_self_intersection(throw_on_self_intersection)
                .clip_volume(clip_volume)
                .use_compact_clipper(use_compact_clipper));
      },
      py::arg("mesh").none(false), py::arg("plane").none(false),
      py::arg("throw_on_self_intersection") = false,
      py::arg("clip_volume") = true, py::arg("use_compact_clipper") = false);
}
}  // namespace pyCGAL
