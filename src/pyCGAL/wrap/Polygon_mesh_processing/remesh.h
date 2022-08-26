#pragma once

// FIXME: this is a workaround for a missing include in
// Polygon_mesh_processing/include/CGAL/Polygon_mesh_processing/internal/Isotropic_remeshing/remesh_impl.h
// in CGAL v5.4.1
#include <CGAL/Kernel/global_functions_3.h>
#include <CGAL/Polygon_mesh_processing/remesh.h>
#include <pyCGAL/typedefs.h>
#include <pyCGAL/wrap/utils/container_proxy.h>
#include <pyCGAL/wrap/utils/named_parameters.h>

#include "detail/remesh.h"
#include "utils/parameter_helpers.h"
#include "utils/property_helpers.h"

namespace pyCGAL {

namespace detail = pyCGAL::wrap::Polygon_mesh_processing::detail;
namespace utils = pyCGAL::wrap::utils;

namespace {

template <typename PolygonMesh>
void isotropic_remeshing(
    PolygonMesh& mesh, const double target_edge_length, py::object face_group,
    const int number_of_iterations, py::object edge_is_constrained_map,
    py::object constrained_edges, py::object vertex_is_constrained_map,
    py::object constrained_vertices, const bool protect_constraints,
    const bool collapse_constraints, const bool do_split,
    const bool do_collapse, const bool do_flip, py::object face_patch_map,
    const int number_of_relaxation_steps, const bool relax_constraints,
    const bool do_project, py::object projection_functor) {
  if (!CGAL::is_triangle_mesh(mesh))
    throw std::runtime_error("Only triangle meshes can be remeshed!");

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

  using Vertex_index = typename PolygonMesh::Vertex_index;
  using Point = typename PolygonMesh::Point;
  using Projector = std::function<Point(const Vertex_index)>;
  std::optional<Projector> projector;
  if (!projection_functor.is_none()) {
    projector = [projection_functor, &mesh](const Vertex_index v) -> Point {
      // we need to instantiate P to later cast it with gcc
      py::object P = projection_functor(mesh.point(v));
      return P.cast<Point>();
    };
  }
  auto projection_functor_option = utils::create_optional_parameter(
      pns::projection_functor<Projector>, projector);

  // WARNING: the correct handling of BGL Named_parameters needs all this burden
  //          the point is the underlying parameter type changes with each
  //          modification so that all_parameters is actually a variant
  auto all_parameters = utils::concatenate(
      pns::number_of_iterations(number_of_iterations)
          .protect_constraints(protect_constraints)
          .collapse_constraints(collapse_constraints)
          .do_split(do_split)
          .do_collapse(do_collapse)
          .do_flip(do_flip)
          .number_of_relaxation_steps(number_of_relaxation_steps)
          .relax_constraints(relax_constraints)
          .do_project(do_project || (!projection_functor.is_none())),
      edge_constraints_option, vertex_constraints_option, face_patches_option,
      projection_functor_option);

  if (face_group.is_none()) {
    std::visit(
        [&](auto&& np) {
          CGAL::Polygon_mesh_processing::isotropic_remeshing(
              faces(mesh), target_edge_length, mesh, np);
        },
        all_parameters);
  } else {
    auto group = utils::container_proxy<Face_index>(face_group);
    std::visit(
        [&](auto&& np) {
          CGAL::Polygon_mesh_processing::isotropic_remeshing(
              group.as_range(), target_edge_length, mesh, np);
        },
        all_parameters);
  }

  if (!constrained_edges.is_none()) {
    assert(edge_constraints);
    mesh.remove_property_map(*edge_constraints);
  }
  if (!constrained_vertices.is_none()) {
    assert(vertex_constraints);
    mesh.remove_property_map(*vertex_constraints);
  }
}

template <typename PolygonMesh>
void split_long_edges(PolygonMesh& mesh, const double max_length,
                      py::object edge_group, py::object edge_is_constrained_map,
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

  if (edge_group.is_none()) {
    std::visit(
        [&](auto&& np) {
          CGAL::Polygon_mesh_processing::split_long_edges(edges(mesh),
                                                          max_length, mesh, np);
        },
        all_parameters);
  } else {
    using Edge_index = typename PolygonMesh::Edge_index;
    auto group = utils::container_proxy<Edge_index>(edge_group);
    std::visit(
        [&](auto&& np) {
          CGAL::Polygon_mesh_processing::split_long_edges(group.as_range(),
                                                          max_length, mesh, np);
        },
        all_parameters);
  }

  if (!constrained_edges.is_none()) {
    assert(edge_constraints);
    mesh.remove_property_map(*edge_constraints);
  }
}

}  // namespace

template <typename PolygonMesh>
void wrap_element(detail::remesh<PolygonMesh>, py::module& module) {
  module.def(
      "isotropic_remeshing", &isotropic_remeshing<PolygonMesh>,
      py::arg("mesh").none(false), py::arg("target_edge_length").none(false),
      py::arg("face_group") = py::none(), py::kw_only(),
      py::arg("number_of_iterations") = static_cast<int>(1),
      py::arg("edge_is_constrained_map") = py::none(),
      py::arg("constrained_edges") = py::none(),
      py::arg("vertex_is_constrained_map") = py::none(),
      py::arg("constrained_vertices") = py::none(),
      py::arg("protect_constraints") = false,
      py::arg("collapse_constraints") = true, py::arg("do_split") = true,
      py::arg("do_collapse") = true, py::arg("do_flip") = true,
      py::arg("face_patch_map") = py::none(),
      py::arg("number_of_relaxation_steps") = static_cast<int>(1),
      py::arg("relax_constraints") = false, py::arg("do_project") = false,
      py::arg("projection_functor") = py::none());

  module.def("split_long_edges", &split_long_edges<PolygonMesh>,
             py::arg("mesh").none(false), py::arg("max_length").none(false),
             py::arg("edge_group") = py::none(), py::kw_only(),
             py::arg("edge_is_constrained_map") = py::none(),
             py::arg("constrained_edges") = py::none(),
             py::arg("face_patch_map") = py::none());
}

}  // namespace pyCGAL
