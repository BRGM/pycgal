#pragma once

#include <CGAL/Polygon_mesh_processing/triangulate_faces.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/boost/graph/Euler_operations.h>
#include <CGAL/boost/graph/helpers.h>

namespace pyCGAL::wrap::utils {

// NB: it's ok to copy (pass) property maps because they are lightweight

namespace detail {

template <typename Surface_mesh>
using Vertex_flag = typename Surface_mesh::template Property_map<
    typename Surface_mesh::Vertex_index, bool>;

template <typename Surface_mesh>
using Edge_flag = typename Surface_mesh::template Property_map<
    typename Surface_mesh::Edge_index, bool>;

template <typename Surface_mesh>
using Vertex_map = typename Surface_mesh::template Property_map<
    typename Surface_mesh::Vertex_index, double>;

template <typename Surface_mesh>
struct Void_property_copier {
  using Edge_index = typename Surface_mesh::Edge_index;
  void operator()(Edge_index, Edge_index) {}
};

template <typename Surface_mesh, typename Insertions>
void _build_insertion_points(const Surface_mesh& sm,
                             const Vertex_map<Surface_mesh> fv,
                             Vertex_flag<Surface_mesh> f_zero,
                             Insertions& insertions) {
  for (auto&& e : sm.edges()) {
    auto v0 = sm.vertex(e, 0);
    auto v1 = sm.vertex(e, 1);
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
}

template <typename Surface_mesh, typename Insertions,
          typename Property_copier = Void_property_copier<Surface_mesh>>
void _insert_points(Surface_mesh& sm, const Insertions& insertions,
                    Vertex_flag<Surface_mesh> f_zero,
                    Property_copier&& property_copier = {}) {
  for (auto&& [e, I] : insertions) {
    auto h = CGAL::Euler::split_edge(sm.halfedge(e), sm);
    property_copier(e, sm.edge(h));
    auto v = sm.target(h);
    sm.point(v) = I;
    f_zero[v] = true;
  }
}

}  // namespace detail

template <typename Edge_property_type = bool, typename Surface_mesh, typename F>
void insert_isovalue(Surface_mesh& sm, F&& f,
                     typename Surface_mesh::template Property_map<
                         typename Surface_mesh::Edge_index, Edge_property_type>
                         edge_property,
                     Edge_property_type iso_edge_value) {
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

  detail::_build_insertion_points(sm, fv, f_zero, insertions);

  detail::_insert_points(
      sm, insertions, f_zero,
      [&sm, edge_property](const Edge_index& e1, const Edge_index& e2) {
        edge_property[e2] = edge_property[e1];
      });

  CGAL::Polygon_mesh_processing::triangulate_faces(sm);
  for (auto&& e : sm.edges()) {
    if (f_zero[sm.vertex(e, 0)] && f_zero[sm.vertex(e, 1)]) {
      assert(edge_property[e] == Edge_property_type{});
      edge_property[e] = iso_edge_value;
    }
  }

  sm.remove_property_map(fv);
  sm.remove_property_map(f_zero);
}

}  // namespace pyCGAL::wrap::utils
