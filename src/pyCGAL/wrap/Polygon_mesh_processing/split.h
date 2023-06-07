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
         py::object constrained_edges, const bool return_vertex_twin,
         const bool return_edge_twin) -> py::object {
        assert(is_valid_polygon_mesh(tm));
        if (!CGAL::is_triangle_mesh(tm))
          throw std::runtime_error("Only triangle meshes can be splitted!");
        if (edge_is_constrained_map.is_none() && constrained_edges.is_none())
          return py::none{};
        // CHECKME: we rely on Surface_mesh property maps not generic pmaps
        using Vertex_index = typename TriangleMesh::Vertex_index;
        using Vertex_twin_map =
            typename TriangleMesh::template Property_map<Vertex_index,
                                                         Vertex_index>;
        using Edge_index = typename TriangleMesh::Edge_index;
        using Edge_twin_map =
            typename TriangleMesh::template Property_map<Edge_index,
                                                         Edge_index>;
        // FIXME: do not use detail namespace
        auto edge_constraints =
            pyCGAL::wrap::utils::detail::optional_flag_map<Edge_index>(
                tm, edge_is_constrained_map, constrained_edges);
        assert(edge_constraints);
        std::optional<Vertex_twin_map> vertex_twin_map;
        bool created = false;
        if (return_vertex_twin) {
          std::tie(vertex_twin_map, created) =
              tm.template add_property_map<Vertex_index, Vertex_index>();
          assert(created);
        }
        std::optional<Edge_twin_map> edge_twin_map;
        if (return_edge_twin) {
          std::tie(edge_twin_map, created) =
              tm.template add_property_map<Edge_index, Edge_index>();
          assert(created);
        }
        wrap::Polygon_mesh_processing::detail::split_along_edges(
            tm, *edge_constraints, get_property_map(boost::vertex_point, tm),
            vertex_twin_map, edge_twin_map);
        if (!constrained_edges.is_none()) {
          tm.remove_property_map(*edge_constraints);
        }
        py::list result;
        if (return_vertex_twin) {
          auto& twin = *vertex_twin_map;
          py::dict pytwin;
          for (auto&& v : tm.vertices()) {
            if (twin[v] != TriangleMesh::null_vertex()) {
              pytwin[py::cast(v)] = py::cast(twin[v]);
            }
          }
          result.append(pytwin);
          tm.remove_property_map(twin);
        }
        if (return_edge_twin) {
          auto& twin = *edge_twin_map;
          py::dict pytwin;
          for (auto&& e : tm.edges()) {
            if (twin[e] != TriangleMesh::null_edge()) {
              pytwin[py::cast(e)] = py::cast(twin[e]);
            }
          }
          result.append(pytwin);
          tm.remove_property_map(twin);
        }
        if (py::len(result) == 0) return py::none{};
        if (py::len(result) == 1) return result[0];
        return result;
      },
      py::arg("tm").none(false), py::kw_only(),
      py::arg("edge_is_constrained_map") = py::none(),
      py::arg("constrained_edges") = py::none(),
      py::arg("return_vertex_twin") = false,
      py::arg("return_edge_twin") = false);
}

}  // namespace pyCGAL
