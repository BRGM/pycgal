#pragma once

#include <CGAL/Polygon_mesh_processing/triangulate_faces.h>
#include <pyCGAL/typedefs.h>

#include "detail/triangulate_faces.h"

namespace pyCGAL {

namespace detail = pyCGAL::wrap::Polygon_mesh_processing::detail;

template <typename PolygonMesh, typename FaceRange>
void wrap_element(detail::triangulate_faces<PolygonMesh, FaceRange>,
                  py::module& module) {
  module.def(
      "triangulate_faces",
      [](PolygonMesh& mesh, py::object face_group) {
        const FaceRange& face_range = face_group.is_none()
                                          ? faces(mesh)
                                          : face_group.cast<const FaceRange&>();
        CGAL::Polygon_mesh_processing::triangulate_faces(face_range, mesh);
      },
      py::arg("mesh").none(false), py::arg("face_group") = py::none());
}

}  // namespace pyCGAL
