#pragma once

#include <pyCGAL/wrap/utils/named_parameters.h>
#include <pybind11/pybind11.h>

#include <optional>

#include "property_helpers.h"

namespace pyCGAL::wrap::utils {

namespace py = pybind11;

namespace detail {

template <typename Index, typename PolygonMesh>
auto optional_flag_map(PolygonMesh& mesh, py::object map, py::object indices) {
  if (!(map.is_none() || indices.is_none())) {
    throw std::runtime_error(
        "You cannot set at the same time a map and a list of indices.");
  }

  using Flag = typename PolygonMesh::template Property_map<Index, bool>;

  std::optional<Flag> flags;

  if (!map.is_none()) {
    flags = utils::convert_to_property_flag<Index>(map, mesh);
  }

  if (!indices.is_none()) {
    auto [is_set, created] = mesh.template add_property_map<Index, bool>();
    assert(created);
    assert(std::none_of(is_set.begin(), is_set.end(), [](auto&& v) {
      return v;
    }));  // C++20: use std::identity
    if (auto p = indices.cast<std::vector<Index>*>()) {
      for (auto&& i : *p) {
        is_set[i] = true;
      }
    } else {
      for (auto&& i : indices) {
        is_set[i.cast<Index>()] = true;
      }
    }
    flags = is_set;
  }

  return flags;
}

}  // namespace detail

template <typename PolygonMesh>
auto edge_is_constrained_map_parameter(PolygonMesh& mesh, py::object edge_map,
                                       py::object edges) {
  using Edge_index = typename PolygonMesh::Edge_index;
  auto edge_constraints =
      detail::optional_flag_map<Edge_index>(mesh, edge_map, edges);
  return std::make_tuple(
      utils::create_optional_parameter(
          CGAL::Polygon_mesh_processing::parameters::edge_is_constrained_map<
              typename std::decay_t<decltype(edge_constraints)>::value_type>,
          edge_constraints),
      edge_constraints);
}

template <typename PolygonMesh>
auto vertex_is_constrained_map_parameter(PolygonMesh& mesh,
                                         py::object vertex_map,
                                         py::object vertices) {
  using Vertex_index = typename PolygonMesh::Vertex_index;
  auto vertex_constraints =
      detail::optional_flag_map<Vertex_index>(mesh, vertex_map, vertices);
  return std::make_tuple(
      utils::create_optional_parameter(
          CGAL::Polygon_mesh_processing::parameters::vertex_is_constrained_map<
              typename std::decay_t<decltype(vertex_constraints)>::value_type>,
          vertex_constraints),
      vertex_constraints);
}

}  // namespace pyCGAL::wrap::utils
