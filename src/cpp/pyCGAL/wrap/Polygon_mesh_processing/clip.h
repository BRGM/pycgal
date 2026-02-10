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
      [](TriangleMesh& tm, TriangleMesh& clipper,
         const bool throw_on_self_intersection, const bool clip_volume,
         const bool use_compact_clipper, const bool do_not_modify) {
        if (!CGAL::is_triangle_mesh(tm) || !CGAL::is_triangle_mesh(clipper))
          throw std::runtime_error("Only triangle meshes can be clipped!");

        namespace pns = CGAL::Polygon_mesh_processing::parameters;

        return CGAL::Polygon_mesh_processing::clip(
            tm, clipper,
            pns::throw_on_self_intersection(throw_on_self_intersection)
                .clip_volume(clip_volume)
                .use_compact_clipper(use_compact_clipper)
                .do_not_modify(do_not_modify));
      },
      py::arg("tm").none(false), py::arg("clipper").none(false), py::kw_only(),
      py::arg("throw_on_self_intersection") = false,
      py::arg("clip_volume") = false, py::arg("use_compact_clipper") = false,
      py::arg("do_not_modify") = false);

  module.def(
      "clip",
      [](TriangleMesh& mesh, const Plane_3& plane,
         const bool throw_on_self_intersection, const bool clip_volume,
         const bool use_compact_clipper, const bool allow_self_intersections) {
        if (!CGAL::is_triangle_mesh(mesh))
          throw std::runtime_error("Only triangle meshes can be clipped!");

        namespace pns = CGAL::Polygon_mesh_processing::parameters;

        return CGAL::Polygon_mesh_processing::clip(
            mesh, plane,
            pns::throw_on_self_intersection(throw_on_self_intersection)
                .clip_volume(clip_volume)
                .use_compact_clipper(use_compact_clipper)
                .allow_self_intersections(allow_self_intersections));
      },
      py::arg("mesh").none(false), py::arg("plane").none(false), py::kw_only(),
      py::arg("throw_on_self_intersection") = false,
      py::arg("clip_volume") = false, py::arg("use_compact_clipper") = true,
      py::arg("allow_self_intersections") = false);
}
}  // namespace pyCGAL
