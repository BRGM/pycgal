#pragma once

#include <CGAL/Polygon_mesh_processing/remesh.h>
#include <pyCGAL/typedefs.h>

namespace pyCGAL {

namespace PMP {

template <typename PolygonMesh,
          typename FaceRange = typename PolygonMesh::Face_range>
struct isotropic_remeshing {};

}  // namespace PMP

template <typename PolygonMesh, typename FaceRange>
void wrap_algorithm(PMP::isotropic_remeshing<PolygonMesh, FaceRange>,
                    py::module& module) {
  module.def(
      "isotropic_remeshing",
      [](PolygonMesh& mesh, const double target_edge_length,
         py::object face_group, const bool do_project) {
        if (!CGAL::is_triangle_mesh(mesh))
          throw std::runtime_error("Only triangle meshes can be remeshed!");
        auto params = CGAL::Polygon_mesh_processing::parameters::all_default();
        params.do_project(do_project);
        const FaceRange& face_range = face_group.is_none()
                                          ? faces(mesh)
                                          : face_group.cast<const FaceRange&>();
        CGAL::Polygon_mesh_processing::isotropic_remeshing(
            face_range, target_edge_length, mesh, params);
      },
      py::arg("mesh").none(false), py::arg("target_edge_length").none(false),
      py::arg("face_group") = py::none(), py::arg("do_project") = true);
}

}  // namespace pyCGAL
