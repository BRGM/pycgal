#pragma once

#include <boost/graph/adjacency_list.hpp>

namespace pyCGAL {
namespace extensions {
namespace Surface_soup {

template <typename ShElts>
struct Edge_graph {
  using Point = typename ShElts::Point;
  using Vertex_index = typename ShElts::Shared_vertex_index;
  using Edge_index = typename ShElts::Shared_edge_index;
  using Graph =
      boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
                            Vertex_index, Edge_index>;
  using vertex_descriptor = typename Graph::vertex_descriptor;

  Graph graph;
  const ShElts &shared;

  Edge_graph(const ShElts &elts) : shared{elts} {
    std::vector<vertex_descriptor> graph_vertices;
    const auto n = shared.number_of_vertices();
    graph_vertices.reserve(n);
    for (int i = 0; i != n; ++i) {
      graph_vertices.emplace_back(add_vertex(i, graph));
    }
    for (auto &&[extremities, e] : shared.edge_id) {
      add_edge(graph_vertices[extremities.first],
               graph_vertices[extremities.second], e, graph);
    }
  }

  bool is_terminal(vertex_descriptor v) const {
    auto b = out_edges(v, graph).first;
    const auto &incidences = shared.edge_incidences;
    return incidences[graph[*b]] != incidences[graph[*(++b)]];
  }

  auto point(vertex_descriptor v) const { return shared.point(graph[v]); }
};

}  // namespace Surface_soup
}  // namespace extensions
}  // namespace pyCGAL
