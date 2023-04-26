#pragma once

#include <CGAL/Polygon_mesh_processing/bbox.h>
#include <CGAL/Surface_mesh.h>

#include "Exact_vertex_point_map.h"

namespace pyCGAL {
namespace extensions {
namespace Surface_soup {

template <typename Mesh_t>
struct Extended_mesh {
  using Mesh = Mesh_t;
  using Vertex_index = typename Mesh::Vertex_index;
  using Edge_index = typename Mesh::Edge_index;
  using Edge_constraints_map =
      typename Mesh::template Property_map<Edge_index, bool>;
  using Exact_vertex_point_map = detail::Exact_vertex_point_map<Mesh>;
  using Shared_vertex_index = int;
  static constexpr Shared_vertex_index null_shared_vertex = -1;
  using Shared_vertex_id_map =
      typename Mesh::template Property_map<Vertex_index, Shared_vertex_index>;
  using Point = typename Exact_vertex_point_map::Point_3;
  using Exact_point = typename Exact_vertex_point_map::Exact_point_3;
  std::shared_ptr<Mesh> tm;  // underlying pointer must not be invalidated
  Edge_constraints_map ecm;
  Exact_vertex_point_map evpm;
  Shared_vertex_id_map svid;
  Extended_mesh(const Mesh &M,
                std::optional<Edge_constraints_map> constraints = {})
      : tm{std::make_shared<Mesh>(M)} {
    init(constraints);
  }
  Extended_mesh(Mesh &&M, std::optional<Edge_constraints_map> constraints = {})
      : tm{std::make_shared<Mesh>(std::forward<Mesh>(M))} {
    init(constraints);
  }
  void init(std::optional<Edge_constraints_map> constraints) {
    evpm = {*tm, "v:exact_point"};
    if (constraints) {
      ecm = *constraints;
    } else {
      ecm =
          tm->template add_property_map<Edge_index, bool>("e:ecm", false).first;
    }
    svid = tm->template add_property_map<Vertex_index, int>("v:svid",
                                                            null_shared_vertex)
               .first;
  }
  const Point &point(const Vertex_index v) const { return tm->point(v); }
  const Exact_point &exact_point(const Vertex_index v) const {
    return get(evpm, v);
  }
  bool is_constrained(const Edge_index e) const { return get(ecm, e); }
  Shared_vertex_index shared_vertex_index(const Vertex_index v) const {
    return get(svid, v);
  }
  void set_shared_vertex_index(const Vertex_index v,
                               const Shared_vertex_index id) const {
    assert(get(svid, v) == null_shared_vertex);
    assert(id != null_shared_vertex);
    put(svid, v, id);
  }
  bool is_null(const Shared_vertex_index sv) const {
    return sv == null_shared_vertex;
  }
  const Mesh &mesh() const { return *tm; }
  Mesh &mesh() { return *tm; }
  auto bbox() const { return CGAL::Polygon_mesh_processing::bbox(*tm); }
  auto edges() const { return tm->edges(); }
  auto number_of_constrained_edges() const {
    std::size_t n = 0;
    for (auto &&e : edges()) {
      if (is_constrained(e)) ++n;
    }
    return n;
  }
  void constrain_border_edges() {
    auto &mesh = *tm;
    for (auto &&e : mesh.edges()) {
      if (mesh.is_border(e)) put(ecm, e, true);
    }
  }
  std::size_t number_of_border_edges() const {
    auto &mesh = *tm;
    std::size_t n = 0;
    for (auto &&e : mesh.edges()) {
      if (mesh.is_border(e)) ++n;
    }
    return n;
  }
};

}  // namespace Surface_soup
}  // namespace extensions
}  // namespace pyCGAL
