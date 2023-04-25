#pragma once

#include <CGAL/Polygon_mesh_processing/clip.h>
#include <CGAL/Polygon_mesh_processing/corefinement.h>
#include <CGAL/box_intersection_d.h>

namespace pyCGAL {
namespace extensions {
namespace Surface_soup {

template <typename EMesh>
struct Surface_soup {
  using Box = CGAL::Box_intersection_d::Box_d<double, 3>;
  using Extended_mesh = EMesh;
  using Mesh = typename Extended_mesh::Mesh;
  using Constraints_map =
      std::optional<typename Extended_mesh::Edge_constraints_map>;
  struct Mesh_iterator : std::vector<EMesh>::iterator {
    using base = typename std::vector<EMesh>::iterator;
    Mesh &operator*() {
      base &it = *this;
      return it->mesh();
    }
  };
  struct Shared_elements {
    using Point = typename Extended_mesh::Point;
    using Exact_point = typename Extended_mesh::Exact_point;
    Surface_soup *soup = nullptr;
    using Mesh_index = std::size_t;
    using Vertex_index = typename Mesh::Vertex_index;
    using Edge_index = typename Mesh::Edge_index;
    using Shared_vertex_index = typename Extended_mesh::Shared_vertex_index;
    struct Shared_edge : std::pair<Shared_vertex_index, Shared_vertex_index> {
      Shared_edge(const Shared_vertex_index a, const Shared_vertex_index b)
          : std::pair<Shared_vertex_index, Shared_vertex_index>{
                CGAL::make_sorted_pair(a, b)} {}
    };
    using Shared_edge_index = std::size_t;
    struct Original_vertex_info {
      Mesh_index mesh;
      Vertex_index v;
    };
    std::map<Exact_point, Shared_vertex_index> vertex_id;
    std::map<Shared_edge, Shared_edge_index> edge_id;
    std::vector<std::vector<Original_vertex_info>> original_vertices_per_id;
    std::vector<std::vector<Mesh_index>> edge_incidences;

    Point point(const Original_vertex_info &info) const {
      assert(soup);
      return (*soup)[info.mesh].point(info.v);
    }
    Point point(const Shared_vertex_index v) const {
      assert(!original_vertices_per_id[v].empty());
      return point(original_vertices_per_id[v][0]);
    }
    auto number_of_vertices() const { return original_vertices_per_id.size(); }
    Shared_vertex_index collect_new_vertex(const Exact_point &P) {
      assert(vertex_id.size() == original_vertices_per_id.size());
      auto insert_res = vertex_id.insert({P, original_vertices_per_id.size()});
      if (insert_res.second) original_vertices_per_id.emplace_back();
      return insert_res.first->second;
    }
    Shared_vertex_index shared_vertex(const Mesh_index i,
                                      const Vertex_index v) {
      assert(soup);
      auto &emesh = (*soup)[i];
      auto sv = emesh.shared_vertex_index(v);
      if (emesh.is_null(sv)) {
        sv = collect_new_vertex(emesh.exact_point(v));
        emesh.set_shared_vertex_index(v, sv);
        original_vertices_per_id[sv].emplace_back(Original_vertex_info{i, v});
      }
      return sv;
    }
    Shared_edge_index collect_new_edge(const Mesh_index i, const Edge_index e) {
      assert(soup);
      assert(edge_id.size() == edge_incidences.size());
      auto &mesh = (*soup)[i].mesh();
      const auto a = shared_vertex(i, source(e, mesh));
      const auto b = shared_vertex(i, target(e, mesh));
      auto insert_res =
          edge_id.insert({Shared_edge{a, b}, edge_incidences.size()});
      if (insert_res.second) edge_incidences.emplace_back();
      const auto se = insert_res.first->second;
      edge_incidences[se].push_back(i);
      return se;
    }
    void collect(Surface_soup &a_soup) {
      assert(soup == nullptr);
      soup = &a_soup;
      Mesh_index i = 0;
      for (auto &&emesh : *soup) {
        for (auto &&e : emesh.edges()) {
          if (emesh.is_constrained(e)) collect_new_edge(i, e);
        }
        ++i;
      }
      for (auto &&incidences : edge_incidences) {
        std::sort(incidences.begin(), incidences.end());
      }
    }
  };
  std::vector<Extended_mesh> emeshes;
  std::vector<Box> boxes;
  Shared_elements elements;
  Surface_soup(std::vector<std::pair<Mesh, Constraints_map>> &&meshes,
               const bool first_is_clipper = false) {
    for (auto &&M : meshes) emeshes.emplace_back(std::move(M.first), M.second);
    init(first_is_clipper);
  }
  Surface_soup(std::vector<std::pair<Mesh, Constraints_map>> &meshes,
               const bool first_is_clipper = false) {
    for (auto &&M : meshes) emeshes.emplace_back(M.first, M.second);
    init(first_is_clipper);
  }
  auto _corefinement_parameters(const std::size_t i) {
    return CGAL::parameters::edge_is_constrained_map(emeshes[i].ecm)
        .vertex_point_map(emeshes[i].evpm);
  }
  void clip_with_first() {
    if (emeshes.empty()) return;
    auto &clipper = emeshes[0];
    for (std::size_t i = 1; i < emeshes.size(); ++i) {
      CGAL::Polygon_mesh_processing::clip(emeshes[i].mesh(), clipper.mesh(),
                                          _corefinement_parameters(i),
                                          _corefinement_parameters(0));
    }
  }
  void init(const bool first_is_clipper) {
    boxes.reserve(emeshes.size());
    for (auto &&S : emeshes) boxes.emplace_back(S.bbox());
    auto corefine_pair = [&](const Box &box_i, const Box &box_j) {
      const auto offset = first_is_clipper ? 1 : 0;
      auto i = box_i.id() + offset;
      auto j = box_j.id() + offset;
      CGAL::Polygon_mesh_processing::corefine(
          emeshes[i].mesh(), emeshes[j].mesh(), _corefinement_parameters(i),
          _corefinement_parameters(j));
    };
    auto start = boxes.begin();
    if (first_is_clipper) {
      clip_with_first();
      ++start;
    }
    CGAL::box_self_intersection_d(start, boxes.end(), corefine_pair);
    elements.collect(*this);
  }
  const EMesh &operator[](const std::size_t i) const { return emeshes[i]; }
  auto begin() { return emeshes.begin(); }
  auto end() { return emeshes.end(); }
  auto meshes_begin() { return Mesh_iterator{emeshes.begin()}; }
  auto meshes_end() { return Mesh_iterator{emeshes.end()}; }
};

}  // namespace Surface_soup
}  // namespace extensions
}  // namespace pyCGAL
