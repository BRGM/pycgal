#pragma once

#include <CGAL/Polygon_mesh_processing/clip.h>
#include <pyCGAL/typedefs.h>

#include <stdexcept>

#include "detail/split.h"
#include "utils/parameter_helpers.h"
#include "utils/split_along_edges.h"

namespace pyCGAL {

namespace detail = pyCGAL::wrap::Polygon_mesh_processing::detail;

template <typename TriangleMesh, typename Plane_3>
void wrap_element(detail::split<TriangleMesh, Plane_3>, py::module& module) {
  module.def(
      "split",
      [](TriangleMesh& tm, TriangleMesh& splitter) {
        if (!CGAL::is_triangle_mesh(tm) || !CGAL::is_triangle_mesh(splitter))
          throw std::runtime_error("Only triangle meshes can be splitted!");
        CGAL::Polygon_mesh_processing::split(tm, splitter);
      },
      py::arg("tm").none(false), py::arg("splitter").none(false));
  module.def(
      "split",
      [](TriangleMesh& tm, const Plane_3& plane,
         const bool throw_on_self_intersection,
         const bool allow_self_intersections) {
        if (!CGAL::is_triangle_mesh(tm))
          throw std::runtime_error("Only triangle meshes can be splitted!");

        namespace pns = CGAL::Polygon_mesh_processing::parameters;

        CGAL::Polygon_mesh_processing::split(
            tm, plane,
            pns::throw_on_self_intersection(throw_on_self_intersection)
                .clip_volume(allow_self_intersections));
      },
      py::arg("tm").none(false), py::arg("plane").none(false),
      py::arg("throw_on_self_intersection") = false,
      py::arg("allow_self_intersections") = false);
  module.def(
      "split_along_edges",
      [](TriangleMesh& tm, py::object edge_is_constrained_map,
         py::object constrained_edges) {
        if (!CGAL::is_triangle_mesh(tm))
          throw std::runtime_error("Only triangle meshes can be splitted!");
        if (edge_is_constrained_map.is_none() && constrained_edges.is_none())
          return;
        using Edge_index = typename TriangleMesh::Edge_index;
        // FIXME: do not use detail namespace
        auto edge_constraints =
            pyCGAL::wrap::utils::detail::optional_flag_map<Edge_index>(
                tm, edge_is_constrained_map, constrained_edges);
        assert(edge_constraints);
        wrap::Polygon_mesh_processing::detail::split_along_edges(
            tm, *edge_constraints, get_property_map(boost::vertex_point, tm));
        if (!constrained_edges.is_none()) {
          tm.remove_property_map(*edge_constraints);
        }
      },
      py::arg("tm").none(false), py::kw_only(),
      py::arg("edge_is_constrained_map") = py::none(),
      py::arg("constrained_edges") = py::none());
}

}  // namespace pyCGAL
