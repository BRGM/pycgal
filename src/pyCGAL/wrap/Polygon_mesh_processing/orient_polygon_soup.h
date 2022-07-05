#pragma once

#include <CGAL/Polygon_mesh_processing/orient_polygon_soup.h>
#include <pyCGAL/typedefs.h>

#include "detail/orient_polygon_soup.h"

namespace pyCGAL {

namespace detail = pyCGAL::wrap::Polygon_mesh_processing::detail;

template <typename PolygonSoup>
void wrap_element(detail::orient_polygon_soup<PolygonSoup>,
                  py::module& module) {
  module.def("orient_polygon_soup", [](PolygonSoup& soup) {
    return CGAL::Polygon_mesh_processing::orient_polygon_soup(soup.points,
                                                              soup.polygons);
  });
}

}  // namespace pyCGAL
