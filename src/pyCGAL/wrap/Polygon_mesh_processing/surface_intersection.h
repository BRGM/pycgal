#pragma once

#include <CGAL/Polygon_mesh_processing/intersection.h>
#include <pyCGAL/typedefs.h>
#include <pyCGAL/wrap/LinearGeometryKernel/extensions/Polyline.h>
#include <pyCGAL/wrap/LinearGeometryKernel/extensions/Polylines.h>
#include <pyCGAL/wrap/utils/named_parameters.h>
#include <pybind11/numpy.h>

#include "detail/surface_intersection.h"

namespace pyCGAL {

namespace detail = pyCGAL::wrap::Polygon_mesh_processing::detail;
namespace utils = pyCGAL::wrap::utils;
namespace eLGK = extensions::LinearGeometryKernel;

template <typename TriangleMesh>
void wrap_element(detail::surface_intersection<TriangleMesh>,
                  py::module& module) {
  import_dependencies<detail::surface_intersection<TriangleMesh>>();

  module.def(
      "surface_intersection",
      [](TriangleMesh& tm1, TriangleMesh& tm2,
         const bool throw_on_self_intersection) {
        if (!CGAL::is_triangle_mesh(tm1) || !CGAL::is_triangle_mesh(tm2))
          throw std::runtime_error("Only triangle meshes can be corefined!");

        using Point = typename TriangleMesh::Point;
        using Polyline = eLGK::Polyline<Point>;
        using Polylines = eLGK::Polylines<Polyline>;

        Polylines polylines;

        CGAL::Polygon_mesh_processing::surface_intersection(
            tm1, tm2, polylines.back_inserter(),
            CGAL::Polygon_mesh_processing::parameters::
                throw_on_self_intersection(throw_on_self_intersection));

        return polylines;
      },
      py::arg("tm1").none(false), py::arg("tm2").none(false), py::kw_only(),
      py::arg("throw_on_self_intersection") = false);
}
}  // namespace pyCGAL
