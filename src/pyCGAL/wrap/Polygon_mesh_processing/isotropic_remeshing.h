#pragma once

#include <CGAL/Polygon_mesh_processing/remesh.h>
#include <pyCGAL/typedefs.h>

#include <optional>

#include "detail/isotropic_remeshing.h"
#include "utils/property_helpers.h"

namespace pyCGAL {

namespace detail = pyCGAL::wrap::Polygon_mesh_processing::detail;
namespace utils = pyCGAL::wrap::utils;

template <typename PolygonMesh, typename FaceRange>
void wrap_element(detail::isotropic_remeshing<PolygonMesh, FaceRange>,
                  py::module& module) {
  module.def(
      "isotropic_remeshing",
      [](PolygonMesh& mesh, const double target_edge_length,
         py::object face_group, const int number_of_iterations,
         py::object edge_is_constrained_map,
         py::object vertex_is_constrained_map, const bool protect_constraints,
         const bool relax_constraints, py::object face_patch_map,
         const bool do_project) {
        if (!CGAL::is_triangle_mesh(mesh))
          throw std::runtime_error("Only triangle meshes can be remeshed!");
        const FaceRange& face_range = face_group.is_none()
                                          ? faces(mesh)
                                          : face_group.cast<const FaceRange&>();
        auto params =
            CGAL::Polygon_mesh_processing::parameters::number_of_iterations(
                number_of_iterations)
                .protect_constraints(protect_constraints)
                .relax_constraints(relax_constraints)
                .do_project(do_project);
        auto edge_constraints =
            utils::convert_to_property_flag<typename PolygonMesh::Edge_index>(
                edge_is_constrained_map, mesh);
        auto vertex_constraints =
            utils::convert_to_property_flag<typename PolygonMesh::Vertex_index>(
                vertex_is_constrained_map, mesh);
        auto face_patches =
            utils::convert_to_property_map<typename PolygonMesh::Face_index,
                                           int>(face_patch_map, mesh, 0);
        // WARNING: the correct handling of BGL Named_parameters needs all these
        // switches
        if (edge_constraints) {
          if (vertex_constraints) {
            if (face_patches) {
              CGAL::Polygon_mesh_processing::isotropic_remeshing(
                  face_range, target_edge_length, mesh,
                  params.edge_is_constrained_map(*edge_constraints)
                      .vertex_is_constrained_map(*vertex_constraints)
                      .face_patch_map(*face_patches));
            } else {
              CGAL::Polygon_mesh_processing::isotropic_remeshing(
                  face_range, target_edge_length, mesh,
                  params.edge_is_constrained_map(*edge_constraints)
                      .vertex_is_constrained_map(*vertex_constraints));
            }
          } else {
            if (face_patches) {
              CGAL::Polygon_mesh_processing::isotropic_remeshing(
                  face_range, target_edge_length, mesh,
                  params.edge_is_constrained_map(*edge_constraints)
                      .face_patch_map(*face_patches));
            } else {
              CGAL::Polygon_mesh_processing::isotropic_remeshing(
                  face_range, target_edge_length, mesh,
                  params.edge_is_constrained_map(*edge_constraints));
            }
          }
        } else {
          if (face_patches) {
            if (vertex_constraints) {
              CGAL::Polygon_mesh_processing::isotropic_remeshing(
                  face_range, target_edge_length, mesh,
                  params.vertex_is_constrained_map(*vertex_constraints)
                      .face_patch_map(*face_patches));
            } else {
              CGAL::Polygon_mesh_processing::isotropic_remeshing(
                  face_range, target_edge_length, mesh,
                  params.face_patch_map(*face_patches));
            }
          } else {
            if (vertex_constraints) {
              CGAL::Polygon_mesh_processing::isotropic_remeshing(
                  face_range, target_edge_length, mesh,
                  params.vertex_is_constrained_map(*vertex_constraints));
            } else {
              CGAL::Polygon_mesh_processing::isotropic_remeshing(
                  face_range, target_edge_length, mesh, params);
            }
          }
        }
      },
      py::arg("mesh").none(false), py::arg("target_edge_length").none(false),
      py::arg("face_group") = py::none(), py::kw_only(),
      py::arg("number_of_iterations") = static_cast<unsigned int>(1),
      py::arg("edge_is_constrained_map") = py::none(),
      py::arg("vertex_is_constrained_map") = py::none(),
      py::arg("protect_constraints") = false,
      py::arg("relax_constraints") = false,
      py::arg("face_patch_map") = py::none(), py::arg("do_project") = false

  );
}

}  // namespace pyCGAL
