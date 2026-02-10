#pragma once

#include <CGAL/Polygon_mesh_processing/intersection.h>
#include <pyCGAL/typedefs.h>

#include "detail/do_intersect.h"

namespace pyCGAL {

namespace detail = pyCGAL::wrap::Polygon_mesh_processing::detail;

template <typename TriangleMesh>
void wrap_element(detail::do_intersect<TriangleMesh>, py::module& module) {
  module.def(
      "do_intersect",
      [](const TriangleMesh& tm1, const TriangleMesh& tm2,
         const bool do_overlap_test_of_bounded_sides) {
        if (!CGAL::is_triangle_mesh(tm1) || !CGAL::is_triangle_mesh(tm2))
          throw std::runtime_error("Only triangle meshes can be corefined!");

        // we explicitely add two named parameters to call the correct template
        // function otherwise do_intersect(const TriangleMesh &, const
        // Polyline&, const NamedParameters&) is called
        const auto params = CGAL::Polygon_mesh_processing::parameters::
            do_overlap_test_of_bounded_sides(do_overlap_test_of_bounded_sides);
        return CGAL::Polygon_mesh_processing::do_intersect(tm1, tm2, params,
                                                           params);
      },
      py::arg("tm1").none(false), py::arg("tm2").none(false), py::kw_only(),
      py::arg("do_overlap_test_of_bounded_sides") = false);
}
}  // namespace pyCGAL
