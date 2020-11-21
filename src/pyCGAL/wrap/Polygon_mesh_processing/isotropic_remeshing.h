#pragma once

#include <CGAL/Polygon_mesh_processing/remesh.h>
#include <pyCGAL/typedefs.h>
#include <pyCGAL/wrap/utils/named_parameters.h>

#include "detail/isotropic_remeshing.h"
#include "utils/property_helpers.h"

namespace pyCGAL {

namespace detail = pyCGAL::wrap::Polygon_mesh_processing::detail;
namespace utils = pyCGAL::wrap::utils;

namespace impl {

template <typename PolygonMesh, typename FaceRange>
void isotropic_remeshing(PolygonMesh& mesh, const double target_edge_length,
                         py::object face_group, const int number_of_iterations,
                         py::object edge_is_constrained_map,
                         py::object vertex_is_constrained_map,
                         const bool protect_constraints,
                         const bool relax_constraints,
                         py::object face_patch_map, const bool do_project) {
  if (!CGAL::is_triangle_mesh(mesh))
    throw std::runtime_error("Only triangle meshes can be remeshed!");

  using Vertex_index = typename PolygonMesh::Vertex_index;
  using Vertex_flag =
      typename PolygonMesh::template Property_map<Vertex_index, bool>;
  using Edge_index = typename PolygonMesh::Edge_index;
  using Edge_flag =
      typename PolygonMesh::template Property_map<Edge_index, bool>;
  using Face_index = typename PolygonMesh::Face_index;
  using Face_label =
      typename PolygonMesh::template Property_map<Face_index, int>;

  const FaceRange& face_range =
      face_group.is_none() ? faces(mesh) : face_group.cast<const FaceRange&>();

  namespace pns = CGAL::Polygon_mesh_processing::parameters;

  auto edge_constraints = utils::convert_to_property_flag<Edge_index>(
      edge_is_constrained_map, mesh);
  auto edge_constraints_option = utils::create_optional_parameter(
      pns::edge_is_constrained_map<Edge_flag>, edge_constraints);

  auto vertex_constraints = utils::convert_to_property_flag<Vertex_index>(
      vertex_is_constrained_map, mesh);
  auto vertex_constraints_option = utils::create_optional_parameter(
      pns::vertex_is_constrained_map<Vertex_flag>, vertex_constraints);

  auto face_patches =
      utils::convert_to_property_map<Face_index, int>(face_patch_map, mesh, 0);
  auto face_patches_option = utils::create_optional_parameter(
      pns::face_patch_map<Face_label>, face_patches);

  // WARNING: the correct handling of BGL Named_parameters needs all this burden
  //          the point is the underlying parameter type changes with each
  //          modification so that all_parameters is actually a variant
  auto all_parameters = utils::concatenate(
      pns::number_of_iterations(number_of_iterations)
          .protect_constraints(protect_constraints)
          .relax_constraints(relax_constraints)
          .do_project(do_project),
      edge_constraints_option, vertex_constraints_option, face_patches_option);

  std::visit(
      [&](auto&& np) {
        CGAL::Polygon_mesh_processing::isotropic_remeshing(
            face_range, target_edge_length, mesh, np);
      },
      all_parameters);
}

}  // namespace impl

template <typename PolygonMesh, typename FaceRange>
void wrap_element(detail::isotropic_remeshing<PolygonMesh, FaceRange>,
                  py::module& module) {
  module.def(
      "isotropic_remeshing", &impl::isotropic_remeshing<PolygonMesh, FaceRange>,
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
