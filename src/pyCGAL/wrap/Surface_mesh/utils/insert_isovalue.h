#pragma once

#include <CGAL/Polygon_mesh_processing/triangulate_faces.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/boost/graph/Euler_operations.h>
#include <CGAL/boost/graph/helpers.h>

namespace pyCGAL::wrap::utils {

template <typename Surface_mesh, typename F>
void insert_isovalue(Surface_mesh& sm, F&& f,
                     std::optional<typename Surface_mesh::template Property_map<
                         typename Surface_mesh::Edge_index, bool>>
                         optional_constrained_edges = {}) {
  using Point = typename Surface_mesh::Point;
  using Vertex_index = typename Surface_mesh::Vertex_index;
  using Edge_index = typename Surface_mesh::Edge_index;
  using Insertion_point = std::pair<Edge_index, Point>;

  assert(CGAL::is_triangle_mesh(sm));

  auto&& [fv, fv_created] =
      sm.template add_property_map<Vertex_index, double>();
  assert(fv_created);
  for (auto&& v : sm.vertices()) {
    fv[v] = f(sm.point(v));
  }

  auto&& [f_zero, f_zero_created] =
      sm.template add_property_map<Vertex_index, bool>();
  assert(f_zero_created);
  std::vector<Insertion_point> insertions;
  for (auto&& e : sm.edges()) {
    Vertex_index v0 = sm.vertex(e, 0);
    Vertex_index v1 = sm.vertex(e, 1);
    double fv0 = fv[v0];
    double fv1 = fv[v1];
    if (fv0 == 0) f_zero[v0] = true;
    if (fv1 == 0) f_zero[v1] = true;
    if (fv0 * fv1 < 0) {
      insertions.emplace_back(
          e, CGAL::barycenter(sm.point(v0), fabs(fv1 / (fv1 - fv0)),
                              sm.point(v1)));
    }
  }

  for (auto&& [e, I] : insertions) {
    auto h = CGAL::Euler::split_edge(sm.halfedge(e), sm);
    if (optional_constrained_edges && (*optional_constrained_edges)[e]) {
      (*optional_constrained_edges)[sm.edge(h)] = true;
    }
    auto v = sm.target(h);
    sm.point(v) = I;
    f_zero[v] = true;
  }

  // collect constrained edges
  if (optional_constrained_edges) {
    CGAL::Polygon_mesh_processing::triangulate_faces(sm);
    auto& constrained_edges = *optional_constrained_edges;
    for (auto&& e : sm.edges()) {
      if (f_zero[sm.vertex(e, 0)] && f_zero[sm.vertex(e, 1)]) {
        constrained_edges[e] = true;
      }
    }
  }

  sm.remove_property_map(fv);
  sm.remove_property_map(f_zero);
}

}  // namespace pyCGAL::wrap::utils
