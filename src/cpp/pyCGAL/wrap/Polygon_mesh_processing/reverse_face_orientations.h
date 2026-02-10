#pragma once

#include <CGAL/Polygon_mesh_processing/orientation.h>
#include <pyCGAL/typedefs.h>
#include <pyCGAL/wrap/utils/named_parameters.h>

#include "detail/reverse_face_orientations.h"

namespace pyCGAL {

namespace detail = pyCGAL::wrap::Polygon_mesh_processing::detail;

template <typename PolygonMesh, typename FaceRange>
void wrap_element(detail::reverse_face_orientations<PolygonMesh, FaceRange>,
                  py::module& module) {
  module.def(
      "reverse_face_orientations",
      &CGAL::Polygon_mesh_processing::reverse_face_orientations<PolygonMesh>);
  module.def(
      "reverse_face_orientations",
      &CGAL::Polygon_mesh_processing::reverse_face_orientations<PolygonMesh,
                                                                FaceRange>);
}

}  // namespace pyCGAL
