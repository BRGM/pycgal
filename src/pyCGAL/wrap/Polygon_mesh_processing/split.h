#pragma once

#include <CGAL/Polygon_mesh_processing/clip.h>
#include <pyCGAL/typedefs.h>

#include "detail/split.h"

namespace pyCGAL {

namespace detail = pyCGAL::wrap::Polygon_mesh_processing::detail;

template <typename TriangleMesh>
void wrap_element(detail::split<TriangleMesh>, py::module& module) {
  module.def(
      "split",
      [](TriangleMesh& tm, TriangleMesh& splitter) {
        CGAL::Polygon_mesh_processing::split(tm, splitter);
      },
      py::arg("tm").none(false), py::arg("splitter").none(false));
}

}  // namespace pyCGAL
