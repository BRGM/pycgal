#pragma once

#include <CGAL/Polygon_mesh_processing/stitch_borders.h>
#include <pyCGAL/typedefs.h>

#include "detail/stitch_borders.h"

namespace pyCGAL {

namespace detail = pyCGAL::wrap::Polygon_mesh_processing::detail;

template <typename PolygonMesh>
void wrap_element(detail::stitch_borders<PolygonMesh>, py::module& module) {
  module.def("stitch_borders", [](PolygonMesh& mesh) {
    CGAL::Polygon_mesh_processing::stitch_borders(mesh);
  });
}

}  // namespace pyCGAL
