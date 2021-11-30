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
        if (face_group.is_none()) {
          CGAL::Polygon_mesh_processing::triangulate_faces(mesh);
        } else {
          using Face_index = typename PolygonMesh::Face_index;
          auto& v =
              face_group.cast<std::vector<Face_index>&>();  // may throw
                                                            // py::cast_error
          CGAL::Polygon_mesh_processing::triangulate_faces(
              CGAL::make_range(begin(v), end(v)), mesh);
        }
      },
      py::arg("mesh").none(false), py::arg("face_group") = py::none());
}

}  // namespace pyCGAL
