#pragma once

#include <CGAL/Polygon_mesh_processing/remesh.h>
#include <pyCGAL/typedefs.h>

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
        if (!edge_is_constrained_map.is_none()) {
          using Edge_index = typename PolygonMesh::Edge_index;
          params.edge_is_constrained_map(
              *utils::convert_to_property_flag<Edge_index>(
                  edge_is_constrained_map, mesh));
        }
        if (!vertex_is_constrained_map.is_none()) {
          using Vertex_index = typename PolygonMesh::Vertex_index;
          params.vertex_is_constrained_map(
              *utils::convert_to_property_flag<Vertex_index>(
                  vertex_is_constrained_map, mesh));
        }
        if (!face_patch_map.is_none()) {
          using Face_index = typename PolygonMesh::Face_index;
          params.face_patch_map(
              *utils::convert_to_property_map<Face_index, PolygonMesh, int>(
                  face_patch_map, mesh, 0));
        }
        CGAL::Polygon_mesh_processing::isotropic_remeshing(
            face_range, target_edge_length, mesh, params);
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
