#pragma once

// FIXME: this is to avoid a bug in the boost template declareation of deque
#include <CGAL/Polygon_mesh_processing/repair_polygon_soup.h>
#include <pyCGAL/typedefs.h>
#include <pyCGAL/wrap/utils/named_parameters.h>

#include <deque>

#include "detail/repair_polygon_soup.h"
#include "utils/parameter_helpers.h"
#include "utils/property_helpers.h"

namespace pyCGAL {

namespace detail = pyCGAL::wrap::Polygon_mesh_processing::detail;
namespace utils = pyCGAL::wrap::utils;

template <typename PolygonSoup>
void wrap_element(detail::repair_polygon_soup<PolygonSoup>,
                  py::module& module) {
  module.def(
      "repair_polygon_soup",
      [](PolygonSoup& soup, const bool erase_all_duplicates,
         const bool require_same_orientation) {
        namespace pns = CGAL::Polygon_mesh_processing::parameters;
        CGAL::Polygon_mesh_processing::repair_polygon_soup(
            soup.points, soup.polygons,
            pns::erase_all_duplicates(erase_all_duplicates)
                .require_same_orientation(require_same_orientation));
      },
      py::arg("soup").none(false), py::kw_only(),
      py::arg("erase_all_duplicates") = false,
      py::arg("require_same_orientation") = false);
}

}  // namespace pyCGAL
