#pragma once

#include <CGAL/Polygon_mesh_processing/corefinement.h>
#include <pyCGAL/typedefs.h>

#include <stdexcept>

namespace pyCGAL {

namespace PMP {

template <typename TriangleMesh, typename Plane_3>
struct corefine {};

}  // namespace PMP

template <typename TriangleMesh>
void wrap_algorithm(PMP::corefine<TriangleMesh, TriangleMesh>,
                    py::module& module) {
  module.def(
      "corefine",
      [](TriangleMesh& tm1, TriangleMesh& tm2,
         const bool throw_on_self_intersection) {
#ifndef NDEBUG
        if (!CGAL::is_triangle_mesh(tm1) || !CGAL::is_triangle_mesh(tm2))
          throw std::runtime_error("Only triangle meshes can be corefined!");
#endif
        auto np1 = CGAL::Polygon_mesh_processing::parameters::all_default();
        np1.throw_on_self_intersection(throw_on_self_intersection);
        auto np2 = np1;
        return CGAL::Polygon_mesh_processing::corefine(tm1, tm2, np1, np2);
      },
      py::arg("tm1").none(false), py::arg("tm2").none(false),
      py::arg("throw_on_self_intersection") = false);
}
}  // namespace pyCGAL
