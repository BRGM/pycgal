#pragma once

#include <CGAL/Polygon_mesh_processing/clip.h>
#include <pyCGAL/typedefs.h>

#include <stdexcept>

namespace pyCGAL {

namespace PMP {

template <typename TriangleMesh, typename Plane_3>
struct clip {};

}  // namespace PMP

template <typename TriangleMesh, typename Plane_3>
void wrap_algorithm(PMP::clip<TriangleMesh, Plane_3>, py::module& module) {
  module.def(
      "clip",
      [](TriangleMesh& mesh, const Plane_3& plane,
         const bool throw_on_self_intersection, const bool clip_volume,
         const bool use_compact_clipper) {
#ifndef NDEBUG
        if (!CGAL::is_triangle_mesh(mesh))
          throw std::runtime_error("Only triangle meshes can be clipped!");
#endif
        auto params = CGAL::Polygon_mesh_processing::parameters::all_default();
        params.throw_on_self_intersection(throw_on_self_intersection);
        params.clip_volume(clip_volume);
        params.use_compact_clipper(use_compact_clipper);
        CGAL::Polygon_mesh_processing::clip(mesh, plane, params);
      },
      py::arg("mesh").none(false), py::arg("plane").none(false),
      py::arg("throw_on_self_intersection") = false,
      py::arg("clip_volume") = true, py::arg("use_compact_clipper") = false);
}
}  // namespace pyCGAL
