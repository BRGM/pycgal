#pragma once

#include <CGAL/Polygon_mesh_processing/bbox.h>
#include <CGAL/boost/graph/helpers.h>
#include <pyCGAL/typedefs.h>

#include "detail/helpers.h"

namespace pyCGAL {

namespace detail = pyCGAL::wrap::Polygon_mesh_processing::detail;

template <typename PolygonMesh>
void wrap_element(detail::helpers<PolygonMesh>, py::module& module) {
  module.def("is_closed", &CGAL::is_closed<PolygonMesh>);
  module.def("is_triangle_mesh", &CGAL::is_triangle_mesh<PolygonMesh>);
  module.def("is_quad_mesh", &CGAL::is_quad_mesh<PolygonMesh>);
  module.def("bbox", &CGAL::Polygon_mesh_processing::bbox<PolygonMesh>);
}

}  // namespace pyCGAL
