#pragma once

#include <CGAL/Polygon_mesh_processing/corefinement.h>
#include <CGAL/boost/graph/iterator.h>
#include <pyCGAL/typedefs.h>
#include <pyCGAL/wrap/utils/named_parameters.h>

#include "detail/corefine.h"
#include "utils/polyline_visitor.h"
#include "utils/property_helpers.h"

namespace pyCGAL {

namespace detail = pyCGAL::wrap::Polygon_mesh_processing::detail;
namespace utils = pyCGAL::wrap::utils;

namespace {

template <typename I, typename T, typename Surface_mesh>
auto create_map(Surface_mesh& sm, T value) {
  auto [map, created] =
      sm.template add_property_map<I, T>(std::string(), value);
  assert(created);
  return map;
};

}  // namespace

template <typename Surface_mesh, typename Twins>
auto collect_polyline_halfedges(Surface_mesh& sm1, Surface_mesh& sm2,
                                const Twins& twins) {
  using Vertex_index = typename Surface_mesh::Vertex_index;
  using Halfedge_index = typename Surface_mesh::Halfedge_index;
  using Edge_index = typename Surface_mesh::Edge_index;

  auto on_polyline = create_map<Vertex_index, bool>(sm1, false);
  auto twin =
      create_map<Vertex_index, Vertex_index>(sm1, Surface_mesh::null_vertex());
  for (auto&& pair : twins) {
    auto [v1, v2] = pair;
    on_polyline[v1] = true;
    twin[v1] = v2;
  }

  auto is_corner = create_map<Vertex_index, bool>(sm1, false);
  auto is_collected = create_map<Edge_index, bool>(sm1, false);
  std::vector<Edge_index> all_edges;
  for (auto&& pair : twins) {
    auto v1 = pair.first;
    int degree = 0;
    for (auto&& h1 : halfedges_around_source(v1, sm1)) {
      if (on_polyline[sm1.target(h1)]) {
        auto e1 = sm1.edge(h1);
        if (!is_collected[e1]) {
          all_edges.emplace_back(e1);
          is_collected[e1] = true;
        }
        ++degree;
      }
    }
    if (degree == 1 || degree > 2) is_corner[v1] = true;
  }
  auto halfedge_twins = [&](const Halfedge_index h1) {
    assert(sm1.is_valid(h1));
    auto h2 = sm2.halfedge(twin[sm1.source(h1)], twin[sm1.target(h1)]);
    assert(sm2.is_valid(h2));
    return std::make_pair(h1, h2);
  };
  auto collect_halfedges_to_corner = [&](Halfedge_index h1, auto out) {
    while (!is_corner[sm1.target(h1)]) {
      assert(!is_collected[sm1.edge(h1)]);
      auto h = h1;
      while (!is_collected[sm1.edge(h)]) {
        h = sm1.next_around_target(h);
        if (h == h1) return;  // we may have encountered a cycle
      }
      h1 = sm1.opposite(h);  // to circulate around next target
      auto e1 = sm1.edge(h1);
      is_collected[e1] = false;
      (*out) = h1;
    }
  };
  std::vector<std::vector<std::pair<Halfedge_index, Halfedge_index>>> polylines;
  for (auto&& e1 : all_edges) {
    if (is_collected[e1]) {
      std::list<Halfedge_index> polyline;
      auto h1 = sm1.halfedge(e1);
      polyline.push_back(h1);
      is_collected[e1] = false;
      collect_halfedges_to_corner(h1, back_inserter(polyline));
      // std::list::iterator stays valid when inserting new elements
      auto reverse_orientation = begin(polyline);
      collect_halfedges_to_corner(sm1.opposite(h1), front_inserter(polyline));
      for (auto ph = begin(polyline); ph != reverse_orientation; ++ph) {
        assert(sm1.is_valid(*ph));
        (*ph) = sm1.opposite(*ph);
      }
      polylines.emplace_back();
      auto& polyline_twins = polylines.back();
      polyline_twins.reserve(size(polyline));
      for (auto&& h1 : polyline) {
        polyline_twins.emplace_back(halfedge_twins(h1));
      }
    }
  }
  // cleaning
  sm1.remove_property_map(on_polyline);
  sm1.remove_property_map(is_corner);
  sm1.remove_property_map(twin);
  sm1.remove_property_map(is_collected);
  return polylines;
}

template <typename TriangleMesh>
void wrap_element(detail::corefine<TriangleMesh>, py::module& module) {
  module.def(
      "corefine",
      [](TriangleMesh& tm1, TriangleMesh& tm2,
         py::object edge_is_constrained_map1,
         py::object edge_is_constrained_map2,
         const bool throw_on_self_intersection, const bool return_new_polylines,
         const bool return_intersection_vertices,
         py::str polyline_id_map) -> py::object {
        if (!CGAL::is_triangle_mesh(tm1) || !CGAL::is_triangle_mesh(tm2))
          throw std::runtime_error("Only triangle meshes can be corefined!");

        using Edge_index = typename TriangleMesh::Edge_index;
        using Edge_flag =
            typename TriangleMesh::template Property_map<Edge_index, bool>;

        namespace pns = CGAL::Polygon_mesh_processing::parameters;

        auto edge_constraints1 = utils::convert_to_property_flag<Edge_index>(
            edge_is_constrained_map1, tm1);
        auto edge_constraints_option1 = utils::create_optional_parameter(
            pns::edge_is_constrained_map<Edge_flag>, edge_constraints1);

        auto edge_constraints2 = utils::convert_to_property_flag<Edge_index>(
            edge_is_constrained_map2, tm2);
        auto edge_constraints_option2 = utils::create_optional_parameter(
            pns::edge_is_constrained_map<Edge_flag>, edge_constraints2);

        using Visitor = utils::Polyline_visitor<TriangleMesh>;
        std::optional<Visitor> visitor;
        typename Visitor::Twins_container twins;
        if (return_new_polylines || return_intersection_vertices ||
            py::len(polyline_id_map) > 0) {
          visitor.emplace(tm1, tm2);
          if (py::len(polyline_id_map) > 0) {
            visitor->set_pid_map(polyline_id_map);
          }
          if (return_intersection_vertices || return_new_polylines) {
            visitor->collect_twins(twins);
          }
        }
        auto polyline_options =
            utils::create_optional_parameter(pns::visitor<Visitor>, visitor);

        // WARNING: the handling of BGL Named_parameters needs all this burden
        std::visit(
            [&](auto&& np1, auto&& np2) {
              CGAL::Polygon_mesh_processing::corefine(tm1, tm2, np1, np2);
            },
            utils::concatenate(
                pns::throw_on_self_intersection(throw_on_self_intersection),
                edge_constraints_option1, polyline_options),
            utils::concatenate(
                pns::throw_on_self_intersection(throw_on_self_intersection),
                edge_constraints_option2));

        py::list result = py::list();

        if (return_new_polylines) {
          py::list polylines_as_halfedges;
          auto intersection_polylines =
              collect_polyline_halfedges(tm1, tm2, twins);
          for (auto& polyline : intersection_polylines) {
            py::list twin_halfedges;
            for (auto& pair : polyline) {
              twin_halfedges.append(py::make_tuple(pair.first, pair.second));
            }
            polylines_as_halfedges.append(twin_halfedges);
          }
          result.append(polylines_as_halfedges);
        }

        if (return_intersection_vertices) {
          py::list vertices;
          for (auto&& pair : twins) {
            vertices.append(py::make_tuple(pair.first, pair.second));
          }
          result.append(vertices);
        }

        const auto nb_results = py::len(result);
        if (nb_results > 1) return result;
        if (nb_results == 1) return result[0];
        return py::none{};
      },
      py::arg("tm1").none(false), py::arg("tm2").none(false), py::kw_only(),
      py::arg("edge_is_constrained_map1") = py::none(),
      py::arg("edge_is_constrained_map2") = py::none(),
      py::arg("throw_on_self_intersection") = false,
      py::arg("return_new_polylines") = false,
      py::arg("return_intersection_vertices") = false,
      py::arg("polyline_id_map") = "");
}
}  // namespace pyCGAL
