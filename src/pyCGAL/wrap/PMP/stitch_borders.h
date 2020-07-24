#pragma once

#include <CGAL/Polygon_mesh_processing/stitch_borders.h>
#include <pyCGAL/typedefs.h>

namespace pyCGAL {

namespace PMP {

template <typename PolygonMesh>
struct stitch_borders {};

}  // namespace PMP

template <typename PolygonMesh>
void wrap_algorithm(PMP::stitch_borders<PolygonMesh>, py::module& module) {
  module.def("stitch_borders",
             &CGAL::Polygon_mesh_processing::stitch_borders<PolygonMesh>);
}

}  // namespace pyCGAL
