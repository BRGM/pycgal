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
  using Mesh_with_constraints = std::pair<Mesh, Constraints_map>;
  struct Mesh_iterator : std::vector<EMesh>::iterator {
    using base = typename std::vector<EMesh>::iterator;
    Mesh& operator*() {
      base& it = *this;
      return it->mesh();
    }
  };
  struct Shared_elements {
    using Point = typename Extended_mesh::Point;
    using Exact_point = typename Extended_mesh::Exact_point;
    Surface_soup* soup = nullptr;
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

    Point point(const Original_vertex_info& info) const {
      assert(soup);
      return (*soup)[info.mesh].point(info.v);
    }
    Point point(const Shared_vertex_index v) const {
      assert(!original_vertices_per_id[v].empty());
      return point(original_vertices_per_id[v][0]);
    }
    auto number_of_vertices() const { return original_vertices_per_id.size(); }
    Shared_vertex_index collect_new_vertex(const Exact_point& P) {
      assert(vertex_id.size() == original_vertices_per_id.size());
      auto insert_res = vertex_id.insert({P, original_vertices_per_id.size()});
      if (insert_res.second) original_vertices_per_id.emplace_back();
      return insert_res.first->second;
    }
    Shared_vertex_index shared_vertex(const Mesh_index i,
                                      const Vertex_index v) {
      assert(soup);
      auto& emesh = (*soup)[i];
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
      auto& mesh = (*soup)[i].mesh();
      const auto a = shared_vertex(i, source(e, mesh));
      const auto b = shared_vertex(i, target(e, mesh));
      auto insert_res =
          edge_id.insert({Shared_edge{a, b}, edge_incidences.size()});
      if (insert_res.second) edge_incidences.emplace_back();
      const auto se = insert_res.first->second;
      edge_incidences[se].push_back(i);
      return se;
    }
    void collect(Surface_soup& a_soup) {
      assert(soup == nullptr);
      soup = &a_soup;
      Mesh_index i = 0;
      for (auto&& emesh : *soup) {
        for (auto&& e : emesh.edges()) {
          if (emesh.is_constrained(e)) collect_new_edge(i, e);
        }
        ++i;
      }
      for (auto&& incidences : edge_incidences) {
        std::sort(incidences.begin(), incidences.end());
      }
    }
  };
  std::vector<Extended_mesh> emeshes;
  std::vector<Box> boxes;
  Shared_elements elements;
  Surface_soup(std::vector<Mesh_with_constraints>&& meshes,
               const bool clip_with_first = false,
               const bool add_borders_as_constraints = true) {
    for (auto&& M : meshes) emeshes.emplace_back(std::move(M.first), M.second);
    init(clip_with_first, add_borders_as_constraints);
  }
  Surface_soup(std::vector<Mesh_with_constraints>& meshes,
               const bool clip_with_first = false,
               const bool add_borders_as_constraints = true) {
    for (auto&& M : meshes) emeshes.emplace_back(M.first, M.second);
    init(clip_with_first, add_borders_as_constraints);
  }
  auto _corefinement_parameters(const std::size_t i) {
    return CGAL::parameters::edge_is_constrained_map(emeshes[i].ecm)
        .vertex_point_map(emeshes[i].evpm);
  }
  void clip_with_first_surface() {
    if (emeshes.empty()) return;
    auto& clipper = emeshes[0];
    for (std::size_t i = 1; i < emeshes.size(); ++i) {
      CGAL::Polygon_mesh_processing::clip(emeshes[i].mesh(), clipper.mesh(),
                                          _corefinement_parameters(i),
                                          _corefinement_parameters(0));
    }
  }
  void init(const bool clip_with_first = false,
            const bool add_borders_as_constraints = true) {
    if (emeshes.empty()) return;
    if (add_borders_as_constraints) {
      assert((!clip_with_first) || emeshes[0].number_of_border_edges() == 0);
      for (std::size_t i = clip_with_first ? 1 : 0; i < emeshes.size(); ++i) {
        emeshes[i].constrain_border_edges();
      }
    }
    boxes.reserve(emeshes.size());
    for (auto&& S : emeshes) boxes.emplace_back(S.bbox());
    auto corefine_pair = [&](const Box& box_i, const Box& box_j) {
      auto i = box_i.id();
      auto j = box_j.id();
      CGAL::Polygon_mesh_processing::corefine(
          emeshes[i].mesh(), emeshes[j].mesh(), _corefinement_parameters(i),
          _corefinement_parameters(j));
    };
    auto start = boxes.begin();
    if (clip_with_first) {
      clip_with_first_surface();
      ++start;
    }
    CGAL::box_self_intersection_d(start, boxes.end(), corefine_pair);
    // the folowing is important as it will provide shared vertex ids
    // from constrained edges (resulting from the corefinement)
    elements.collect(*this);
  }
  const EMesh& operator[](const std::size_t i) const { return emeshes[i]; }
  auto begin() { return emeshes.begin(); }
  auto end() { return emeshes.end(); }
  auto meshes_begin() { return Mesh_iterator{emeshes.begin()}; }
  auto meshes_end() { return Mesh_iterator{emeshes.end()}; }
  auto as_brep() {
    using Point = typename Extended_mesh::Point;
    using Vertex_index = typename Mesh::Vertex_index;
    using Face_index = typename Mesh::Face_index;
    using Shared_vertex_index = typename Extended_mesh::Shared_vertex_index;
    using Triangle = std::array<Shared_vertex_index, 3>;
    int component_offset = 0;
    std::size_t nb_vertices = elements.number_of_vertices();
    std::size_t nb_facets = 0;
    for (auto&& em : emeshes) {
      nb_vertices +=
          em.mesh().number_of_vertices() - em.number_of_shared_vertices();
      nb_facets += em.mesh().number_of_faces();
    }
    std::vector<Point> vertices;
    vertices.resize(nb_vertices);
    std::vector<Triangle> facets;
    facets.resize(nb_facets);
    std::vector<int> facet_index;
    facet_index.resize(nb_facets);
    Shared_vertex_index kv = 0;
    std::size_t kf = 0;
    int comp_offset = 0;
    for (; kv < elements.number_of_vertices(); ++kv) {
      vertices[kv] = elements.point(kv);
    }
    for (auto&& em : emeshes) {
      auto& m = em.mesh();
      auto [vid, vid_created] =
          m.template add_property_map<Vertex_index, Shared_vertex_index>();
      assert(vid_created);
      auto [comp, comp_created] =
          m.template add_property_map<Face_index, int>();
      assert(comp_created);
      for (auto&& v : m.vertices()) {
        if (!em.is_shared(v)) {
          vertices[kv] = m.point(v);
          vid[v] = kv;
          ++kv;
        } else {
          vid[v] = em.shared_id(v);
        }
      }
      auto nbcomp = CGAL::Polygon_mesh_processing::connected_components(
          m, comp, CGAL::parameters::edge_is_constrained_map(em.ecm));
      assert(CGAL::is_triangle_mesh(m));
      for (auto&& f : m.faces()) {
        int i = 0;
        for (auto&& v : CGAL::vertices_around_face(m.halfedge(f), m)) {
          facets[kf][i] = vid[v];
          ++i;
        }
        facet_index[kf] = comp_offset + comp[f];
        ++kf;
      }
      comp_offset += nbcomp;
      m.remove_property_map(vid);
      m.remove_property_map(comp);
    }
    assert(kv == vertices.size());
    assert(kf == facets.size());
    assert(kf == facet_index.size());
    return std::make_tuple(move(vertices), move(facets), move(facet_index));
  }
};

}  // namespace Surface_soup
}  // namespace extensions
}  // namespace pyCGAL
