#pragma once

#include <CGAL/Polygon_mesh_processing/remesh.h>
#include <pyCGAL/typedefs.h>
#include <pyCGAL/wrap/utils/named_parameters.h>

#include "detail/remesh.h"
#include "utils/parameter_helpers.h"
#include "utils/property_helpers.h"

namespace pyCGAL {

namespace detail = pyCGAL::wrap::Polygon_mesh_processing::detail;
namespace utils = pyCGAL::wrap::utils;

namespace {

template <typename PolygonMesh, typename FaceRange>
void isotropic_remeshing(PolygonMesh& mesh, const double target_edge_length,
                         py::object face_group, const int number_of_iterations,
                         py::object edge_is_constrained_map,
                         py::object constrained_edges,
                         py::object vertex_is_constrained_map,
                         py::object constrained_vertices,
                         const bool protect_constraints,
                         const bool relax_constraints,
                         py::object face_patch_map, const bool do_project) {
  if (!CGAL::is_triangle_mesh(mesh))
    throw std::runtime_error("Only triangle meshes can be remeshed!");

  const FaceRange face_range =
      face_group.is_none() ? faces(mesh) : face_group.cast<const FaceRange&>();

  namespace pns = CGAL::Polygon_mesh_processing::parameters;

  auto [edge_constraints_option, edge_constraints] =
      utils::edge_is_constrained_map_parameter(mesh, edge_is_constrained_map,
                                               constrained_edges);

  auto [vertex_constraints_option, vertex_constraints] =
      utils::vertex_is_constrained_map_parameter(
          mesh, vertex_is_constrained_map, constrained_vertices);

  using Face_index = typename PolygonMesh::Face_index;
  using Face_label =
      typename PolygonMesh::template Property_map<Face_index, int>;
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

  if (!constrained_edges.is_none()) {
    assert(edge_constraints);
    mesh.remove_property_map(*edge_constraints);
  }
  if (!constrained_vertices.is_none()) {
    assert(vertex_constraints);
    mesh.remove_property_map(*vertex_constraints);
  }
}

template <typename PolygonMesh, typename EdgeRange>
void split_long_edges(PolygonMesh& mesh, const double max_length,
                      const EdgeRange& edge_range,
                      py::object edge_is_constrained_map,
                      py::object constrained_edges, py::object face_patch_map) {
  if (!CGAL::is_triangle_mesh(mesh))
    throw std::runtime_error("Only triangle meshes are handled!");

  namespace pns = CGAL::Polygon_mesh_processing::parameters;

  auto [edge_constraints_option, edge_constraints] =
      utils::edge_is_constrained_map_parameter(mesh, edge_is_constrained_map,
                                               constrained_edges);

  using Face_index = typename PolygonMesh::Face_index;
  using Face_label =
      typename PolygonMesh::template Property_map<Face_index, int>;
  auto face_patches =
      utils::convert_to_property_map<Face_index, int>(face_patch_map, mesh, 0);
  auto face_patches_option = utils::create_optional_parameter(
      pns::face_patch_map<Face_label>, face_patches);

  // WARNING: the correct handling of BGL Named_parameters needs all this burden
  //          the point is the underlying parameter type changes with each
  //          modification so that all_parameters is actually a variant
  auto all_parameters =
      utils::concatenate_options(edge_constraints_option, face_patches_option);

  std::visit(
      [&](auto&& np) {
        CGAL::Polygon_mesh_processing::split_long_edges(edge_range, max_length,
                                                        mesh, np);
      },
      all_parameters);

  if (!constrained_edges.is_none()) {
    assert(edge_constraints);
    mesh.remove_property_map(*edge_constraints);
  }
}

}  // namespace

template <typename PolygonMesh, typename EdgeRange, typename FaceRange>
void wrap_element(detail::remesh<PolygonMesh, EdgeRange, FaceRange>,
                  py::module& module) {
  module.def(
      "isotropic_remeshing", &isotropic_remeshing<PolygonMesh, FaceRange>,
      py::arg("mesh").none(false), py::arg("target_edge_length").none(false),
      py::arg("face_group") = py::none(), py::kw_only(),
      py::arg("number_of_iterations") = static_cast<unsigned int>(1),
      py::arg("edge_is_constrained_map") = py::none(),
      py::arg("constrained_edges") = py::none(),
      py::arg("vertex_is_constrained_map") = py::none(),
      py::arg("constrained_vertices") = py::none(),
      py::arg("protect_constraints") = false,
      py::arg("relax_constraints") = false,
      py::arg("face_patch_map") = py::none(), py::arg("do_project") = false);

  module.def(
      "split_long_edges",
      [](PolygonMesh& self, const double max_length, py::object edge_group,
         py::object edge_is_constrained_map, py::object constrained_edges,
         py::object face_patch_map) {
        if (edge_group.is_none()) {
          split_long_edges(self, max_length, edges(self),
                           edge_is_constrained_map, constrained_edges,
                           face_patch_map);
        } else {
          using Edge_index = typename PolygonMesh::Edge_index;
          auto& v =
              edge_group.cast<std::vector<Edge_index>&>();  // may throw
                                                            // py::cast_error
          split_long_edges(self, max_length, CGAL::make_range(begin(v), end(v)),
                           edge_is_constrained_map, constrained_edges,
                           face_patch_map);
        }
      },
      py::arg("mesh").none(false), py::arg("max_length").none(false),
      py::arg("edge_group") = py::none(), py::kw_only(),
      py::arg("edge_is_constrained_map") = py::none(),
      py::arg("constrained_edges") = py::none(),
      py::arg("face_patch_map") = py::none());
}

}  // namespace pyCGAL
