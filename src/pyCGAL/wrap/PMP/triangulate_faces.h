#pragma once

#include <CGAL/Polygon_mesh_processing/triangulate_faces.h>
#include <pyCGAL/typedefs.h>

namespace pyCGAL {

namespace PMP {

template <typename PolygonMesh,
          typename FaceRange = typename PolygonMesh::Face_range>
struct triangulate_faces {};

}  // namespace PMP

template <typename PolygonMesh, typename FaceRange>
void wrap_algorithm(PMP::triangulate_faces<PolygonMesh, FaceRange>,
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

  module.def("is_triangle_mesh", &CGAL::is_triangle_mesh<PolygonMesh>);
}

}  // namespace pyCGAL
