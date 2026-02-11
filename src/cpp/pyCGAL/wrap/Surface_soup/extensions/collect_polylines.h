#pragma once

#include <CGAL/boost/graph/split_graph_into_polylines.h>
#include <pyCGAL/wrap/LinearGeometryKernel/extensions/Polyline.h>
#include <pyCGAL/wrap/LinearGeometryKernel/extensions/Polylines.h>

#include "Edge_graph.h"

namespace pyCGAL {
namespace extensions {

namespace LGK = LinearGeometryKernel;

namespace Surface_soup {

namespace detail {

template <typename Edge_graph, typename Point>
struct Polylines_collector {
  using vertex_descriptor = typename Edge_graph::Graph::vertex_descriptor;
  using Polyline = LGK::Polyline<Point>;
  const Edge_graph& edge_graph;
  LGK::Polylines<Polyline> polylines;

  Polylines_collector(const Edge_graph& edge_graph)
      : edge_graph(edge_graph), polylines{} {}

  void start_new_polyline() { polylines.start_new_polyline(); }
  void add_node(vertex_descriptor v) {
    polylines.add_node(edge_graph.point(v));
  }
  void end_polyline() {}
};

template <typename Edge_graph>
auto polylines_collector(const Edge_graph& edge_graph) {
  return Polylines_collector<Edge_graph, typename Edge_graph::Point>{
      edge_graph};
}
}  // namespace detail

template <typename Shared>
auto collect_polylines(const Shared& elements) {
  auto edge_graph = Edge_graph{elements};
  auto collector = detail::polylines_collector(edge_graph);
  CGAL::split_graph_into_polylines(edge_graph.graph, collector,
                                   [&edge_graph](auto v, const auto& g) {
                                     assert(&g == &edge_graph.graph);
                                     return edge_graph.is_terminal(v);
                                   });
  return std::move(collector.polylines);
}

}  // namespace Surface_soup
}  // namespace extensions
}  // namespace pyCGAL
