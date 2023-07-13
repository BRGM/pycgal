#pragma once

#include <CGAL/boost/graph/split_graph_into_polylines.h>

// FIXME: this should be in a generic header
#include <pyCGAL/wrap/Polygon_mesh_processing/utils/property_helpers.h>

namespace pyCGAL::wrap::utils {

namespace detail {

template <typename Vertex_index>
struct Polylines_collector {
  std::vector<Vertex_index> all_vertices;
  py::list polylines;
  py::list polyline;

  Polylines_collector(std::vector<Vertex_index>&& vertices)
      : all_vertices{std::forward<std::vector<Vertex_index>>(vertices)} {}

  void start_new_polyline() {
    polyline = py::list{};
    polylines.append(polyline);
  }
  void add_node(int i) { polyline.append(all_vertices[i]); }
  void end_polyline() {}
};

}  // namespace detail

template <typename Surface_mesh>
auto split_edge_constraints_into_polylines(Surface_mesh& mesh,
                                           py::object constraints) {
  using Vertex_index = typename Surface_mesh::Vertex_index;
  using Edge_index = typename Surface_mesh::Edge_index;

  auto is_constrained_edge_option =
      convert_to_property_flag<Edge_index>(constraints, mesh);
  if (!is_constrained_edge_option) {
    throw std::runtime_error("Could not convert object to edge map.");
  }
  const auto& is_constrained_edge = *is_constrained_edge_option;

  auto [keep, keep_created] =
      mesh.template add_property_map<Vertex_index, bool>();
  assert(keep_created);
  std::size_t nb_kept = 0;
  auto collect = [&](auto&& v) {
    if (!keep[v]) {
      keep[v] = true;
      ++nb_kept;
    }
  };
  for (auto&& e : mesh.edges()) {
    if (is_constrained_edge[e]) {
      collect(mesh.vertex(e, 0));
      collect(mesh.vertex(e, 1));
    }
  }
  auto [vid, vid_created] =
      mesh.template add_property_map<Vertex_index, int>("", -1);
  assert(vid_created);
  std::vector<Vertex_index> vertices;
  vertices.resize(nb_kept);
  std::size_t i = 0;
  for (auto&& v : mesh.vertices()) {
    if (keep[v]) {
      vertices[i] = v;
      vid[v] = i;
      ++i;
    }
  }

  using Graph =
      boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS>;
  Graph graph{nb_kept};

  for (auto&& e : mesh.edges()) {
    if (is_constrained_edge[e]) {
      add_edge(vid[mesh.vertex(e, 0)], vid[mesh.vertex(e, 1)], graph);
    }
  }

  mesh.remove_property_map(keep);
  mesh.remove_property_map(vid);

  detail::Polylines_collector<Vertex_index> collector{std::move(vertices)};

  CGAL::split_graph_into_polylines(graph, collector);

  return collector.polylines;
}

}  // namespace pyCGAL::wrap::utils
