#pragma once

#include <CGAL/Polygon_mesh_processing/corefinement.h>

#include <map>
#include <optional>

namespace pyCGAL::wrap::utils {

template <typename Mesh>
using Default_visitor =
    CGAL::Polygon_mesh_processing::Corefinement::Default_visitor<Mesh>;

template <typename Surface_mesh, typename Polyline_id = int>
struct Polyline_visitor : Default_visitor<Surface_mesh> {
  using vertex_descriptor =
      typename Default_visitor<Surface_mesh>::vertex_descriptor;
  using halfedge_descriptor =
      typename Default_visitor<Surface_mesh>::halfedge_descriptor;
  using Vertex_index = typename Surface_mesh::Vertex_index;
  using Halfedge_index = typename Surface_mesh::Halfedge_index;
  using Edge_index = typename Surface_mesh::Edge_index;
  using Polyline_map =
      typename Surface_mesh::template Property_map<Edge_index, Polyline_id>;
  using Twin_map =
      typename Surface_mesh::template Property_map<Vertex_index, Vertex_index>;
  using Twins = std::pair<Vertex_index, Vertex_index>;
  using Twins_container = std::vector<Twins>;
  Surface_mesh* sm1 = nullptr;
  Surface_mesh* sm2 = nullptr;
  Twins_container* ptwins = nullptr;
  // property maps are just pointers underneath: copies are shallow
  std::optional<std::pair<Polyline_map, Polyline_map>> pmaps;
  halfedge_descriptor splitted_halfedge;
  using Monitor =
      std::vector<std::pair<Polyline_map, typename Surface_mesh::Edge_index>>;
  std::shared_ptr<Monitor> monitoring;
#ifndef NDEBUG
  const Surface_mesh* splitted_mesh = nullptr;
#endif
  Polyline_visitor() = delete;
  Polyline_visitor(Surface_mesh& mesh1, Surface_mesh& mesh2)
      : sm1(&mesh1), sm2(&mesh2) {}
  void set_pid_map(std::string name) {
    assert(sm1 && sm2);
    assert(!pmaps);
    auto [map1, exists1] =
        sm1->template property_map<Edge_index, Polyline_id>(name);
    if (!exists1) throw std::runtime_error("Polyline id map must exist !");
    auto [map2, exists2] =
        sm2->template property_map<Edge_index, Polyline_id>(name);
    if (!exists2) throw std::runtime_error("Polyline id map must exist !");
    pmaps.emplace(map1, map2);
  }
  void collect_twins(Twins_container& v) {
    assert(v.empty());
    assert(!ptwins);
    ptwins = &v;
  }
  void _collect_vertex(std::size_t i_id, const Surface_mesh& sm,
                       vertex_descriptor v) {
    assert(ptwins);
    auto& twins = *ptwins;
    assert(i_id < twins.size());
    if (&sm == sm1) {
      assert(twins[i_id].first == Surface_mesh::null_vertex());
      twins[i_id].first = v;
    } else {
      assert(&sm == sm2);
      assert(twins[i_id].second == Surface_mesh::null_vertex());
      twins[i_id].second = v;
    }
  }
  void new_vertex_added(std::size_t i_id, vertex_descriptor v,
                        const Surface_mesh& sm) {
    if (ptwins) {
      _collect_vertex(i_id, sm, v);
    }
  }
  void intersection_point_detected(std::size_t i_id, int sdim,
                                   halfedge_descriptor h_f,
                                   halfedge_descriptor h_e,
                                   const Surface_mesh& sm_f,
                                   const Surface_mesh& sm_e,
                                   bool is_target_coplanar,
                                   bool is_source_coplanar) {
    if (ptwins) {
      auto& twins = *ptwins;
      assert(i_id == twins.size());
      twins.emplace_back(Surface_mesh::null_vertex(),
                         Surface_mesh::null_vertex());
      if (sdim == 0) {
        _collect_vertex(i_id, sm_f, sm_f.target(h_f));
      }
      assert(!(is_target_coplanar && is_source_coplanar));
      if (is_target_coplanar) {
        assert(!is_source_coplanar);
        _collect_vertex(i_id, sm_f, sm_f.target(h_f));
      } else {
        if (is_source_coplanar) {
          assert(!is_target_coplanar);
          _collect_vertex(i_id, sm_f, sm_f.source(h_f));
        }
      }
    }
  }
  void before_edge_split(halfedge_descriptor h, const Surface_mesh& sm) {
#ifndef NDEBUG
    assert(!splitted_mesh);
    assert(&sm == sm1 || &sm == sm2);
    splitted_mesh = &sm;
#endif
    splitted_halfedge = h;
  }
  void edge_split(halfedge_descriptor hnew, const Surface_mesh& sm) {
    assert(splitted_mesh == &sm);
    if (pmaps) {
      auto& pmap = (&sm == sm1) ? pmaps->first : pmaps->second;
      pmap[sm.edge(hnew)] = pmap[sm.edge(splitted_halfedge)];
    }
  }
  void after_edge_split() {
#ifndef NDEBUG
    assert(splitted_mesh);
    splitted_mesh = nullptr;
#endif
  }

  auto vertex_and_polyline_id(const Surface_mesh& tm, const Polyline_map& pmap,
                              Halfedge_index h) {
    if (tm.is_border(h)) {
      return std::make_pair(Surface_mesh::null_vertex(), 0);
    }
    const auto hn = tm.next(h);
    const auto v = tm.target(hn);
    const auto idn = pmap[tm.edge(hn)];
    const auto hp = tm.prev(h);
    assert(tm.source(hp) == v);  // triangle mesh
    const auto idp = pmap[tm.edge(hp)];
    assert(idn >= 0);
    assert(idp >= 0);
    assert((idn != 0 && idp == 0) || (idn == 0 && idp != 0) || (idn == idp));
    return std::make_pair(v, std::max(idn, idp));
  }

  auto update_new_edge(const Surface_mesh& tm, Polyline_map& pmap,
                       Vertex_index a, Vertex_index b, Polyline_id id) {
    assert(a != Surface_mesh::null_vertex());
    if (b == Surface_mesh::null_vertex()) {
      assert(id == 0);
      return;
    }
    const auto h = tm.halfedge(a, b);
    assert(h != Surface_mesh::null_halfedge() && tm.is_valid(h));
    pmap[tm.edge(h)] = id;
  }

  /**/

  bool border_or_triangle_faces(const Surface_mesh& sm, Halfedge_index h) {
    const Halfedge_index oh = sm.opposite(h);
    return (sm.is_border(h) || sm.degree(sm.face(h)) == 3) &&
           (sm.is_border(oh) || sm.degree(sm.face(oh)) == 3);
  }

  void collapse_edges_around_corner(Surface_mesh& sm1, Surface_mesh& sm2,
                                    Twin_map& tmap1, Twin_map& tmap2,
                                    Polyline_map& pmap1, Polyline_map& pmap2,
                                    Vertex_index v1, const double epsilon) {
    assert(!sm1.is_removed(v1));
    assert(sm1.is_valid(v1));
    assert(sm1.is_valid(sm1.halfedge(v1)));
#ifndef NDEBUG
    int n = 0;
    for (auto&& h1 : CGAL::halfedges_around_source(v1, sm1)) {
      const auto w1 = sm1.target(h1);
      auto d = CGAL::squared_distance(sm1.point(v1), sm1.point(w1));
      if (d < epsilon) {
        // std::cerr << "Check - small edge on " << h1 << " with squared length
        // "
        //          << d << std::endl;
        ++n;
      }
    }
    assert(n <= 1);
#endif
    for (auto&& h1 : CGAL::halfedges_around_target(v1, sm1)) {
      assert(sm1.is_valid(h1));
      const auto w1 = sm1.source(h1);
      assert(!sm1.is_removed(w1));
      assert(sm1.is_valid(w1));
      assert(sm1.is_valid(sm1.halfedge(w1)));
      auto d = CGAL::squared_distance(sm1.point(v1), sm1.point(w1));
      if (d < epsilon) {
        // std::cerr << "Small edge on " << h1 << " with squared length " << d
        //          << std::endl;
        assert(border_or_triangle_faces(sm1, h1));
        const auto [nL1, idL1] = vertex_and_polyline_id(sm1, pmap1, h1);
        const auto [nR1, idR1] =
            vertex_and_polyline_id(sm1, pmap1, sm1.opposite(h1));
        const auto w2 = tmap1[w1];
        assert(w2 != Surface_mesh::null_vertex());
        assert(pmap1[sm1.edge(h1)] != 0);
        CGAL::Euler::collapse_edge(sm1.edge(h1), sm1);
        assert(sm1.is_removed(w1) && !sm1.is_removed(v1));
        assert(sm1.is_valid(sm1.halfedge(v1)));
        assert(tmap1[v1] ==
               Surface_mesh::null_vertex());  // CHECKME: a corner can not be
                                              // constrained by more than 3
                                              // surfaces)
        tmap1[v1] = w2;
        tmap2[w2] = v1;
        update_new_edge(sm1, pmap1, v1, nL1, idL1);
        update_new_edge(sm1, pmap1, v1, nR1, idR1);
        return;
      }
    }
  }

  template <typename Iterator>
  void collapse_intersection_edges_around_source(
      Surface_mesh& sm1, Surface_mesh& sm2, Twin_map& tmap1, Twin_map& tmap2,
      Polyline_map& pmap1, Polyline_map& pmap2, Vertex_index v1,
      const double epsilon, Iterator corners1, Iterator corners2) {
    assert(!sm1.is_removed(v1));
    assert(sm1.is_valid(v1));
    assert(sm1.is_valid(sm1.halfedge(v1)));
    std::optional<Vertex_index> iw1;
    for (auto&& h1 : CGAL::halfedges_around_source(v1, sm1)) {
      assert(sm1.is_valid(h1));
      const auto w1 = sm1.target(h1);
      assert(!sm1.is_removed(w1));
      assert(sm1.is_valid(w1));
      assert(sm1.is_valid(sm1.halfedge(w1)));
      auto d = CGAL::squared_distance(sm1.point(v1), sm1.point(w1));
      if (d < epsilon) {
        // std::cerr << "Small edge on mesh 1 " << h1 << " with squared length "
        //          << d << std::endl;
        assert(border_or_triangle_faces(sm1, h1));
        const auto w2 = tmap1[w1];
        if (w2 != Surface_mesh::null_vertex()) {
          assert(!iw1);
          iw1 = w1;
        } else {
          *corners1 = w1;
          ++corners1;
        }
      }
    }
    const auto v2 = tmap1[v1];
    assert(v2 != Surface_mesh::null_vertex());
    assert(tmap2[v2] == v1);
    for (auto&& h2 : CGAL::halfedges_around_source(v2, sm2)) {
      const auto w2 = sm2.target(h2);
      auto d = CGAL::squared_distance(sm2.point(v2), sm2.point(w2));
      if (d < epsilon) {
        // std::cerr << "Small edge on mesh 2 " << h2 << " with squared length "
        //          << d << std::endl;
        assert(border_or_triangle_faces(sm2, h2));
        const auto w1 = tmap2[w2];
        if (w1 != Surface_mesh::null_vertex()) {
          assert(tmap1[w1] == w2);
          assert(iw1 && *iw1 == w1);
        } else {
          *corners2 = w2;
          ++corners2;
        }
      }
    }
    if (iw1) {
      const auto w1 = *iw1;
      const auto h1 = sm1.halfedge(v1, w1);
      assert(h1 != Surface_mesh::null_halfedge());
      const auto [nL1, idL1] = vertex_and_polyline_id(sm1, pmap1, h1);
      const auto [nR1, idR1] =
          vertex_and_polyline_id(sm1, pmap1, sm1.opposite(h1));
      const auto w2 = tmap1[w1];
      assert(w2 != Surface_mesh::null_vertex());
      assert(tmap2[w2] == w1);
      auto h2 = sm2.halfedge(v2, w2);
      assert(sm2.is_valid(h2));
      assert(border_or_triangle_faces(sm2, h2));
      const auto [vL2, idL2] = vertex_and_polyline_id(sm2, pmap2, h2);
      const auto [vR2, idR2] =
          vertex_and_polyline_id(sm2, pmap2, sm2.opposite(h2));
      const auto I = CGAL::midpoint(sm1.point(v1), sm1.point(w1));
      CGAL::Euler::collapse_edge(sm1.edge(h1), sm1);
      CGAL::Euler::collapse_edge(sm2.edge(h2), sm2);
      assert(sm1.is_removed(v1) && !sm1.is_removed(w1));
      assert(sm2.is_removed(v2) && !sm2.is_removed(w2));
      assert(sm1.is_valid(sm1.halfedge(w1)));
      assert(sm2.is_valid(sm2.halfedge(w2)));
      update_new_edge(sm2, pmap2, w2, vL2, idL2);
      update_new_edge(sm2, pmap2, w2, vR2, idR2);
      update_new_edge(sm1, pmap1, w1, nL1, idL1);
      update_new_edge(sm1, pmap1, w1, nR1, idR1);
    }
  }

  void process_small_edges(const double& epsilon) {
    assert(sm1 && sm2);
    assert(ptwins);
    auto& twins = *ptwins;
    assert(pmaps);
    auto& pmap1 = pmaps->first;
    auto& pmap2 = pmaps->second;
    auto [tmap1, ok1] =
        sm1->template add_property_map<Vertex_index, Vertex_index>();
    assert(ok1);
    auto [tmap2, ok2] =
        sm2->template add_property_map<Vertex_index, Vertex_index>();
    assert(ok2);
    for (auto&& pair : twins) {
      auto [v1, v2] = pair;
      tmap1[v1] = v2;
      tmap2[v2] = v1;
    }
    std::set<Vertex_index> corners1;
    std::set<Vertex_index> corners2;
    for (auto&& pair : twins) {
      collapse_intersection_edges_around_source(
          *sm1, *sm2, tmap1, tmap2, pmap1, pmap2, pair.first, epsilon,
          inserter(corners1), inserter(corners2));
    }
    for (auto&& v1 : corners1) {
      collapse_edges_around_corner(*sm1, *sm2, tmap1, tmap2, pmap1, pmap2, v1,
                                   epsilon);
    }
    for (auto&& v2 : corners2) {
      collapse_edges_around_corner(*sm2, *sm1, tmap2, tmap1, pmap2, pmap1, v2,
                                   epsilon);
    }
    // TODO: we could work only with twin maps
    // maybe a single one...
    twins.clear();
    for (auto&& v1 : sm1->vertices()) {
      const auto v2 = tmap1[v1];
      if (v2 != Surface_mesh::null_vertex()) {
        twins.emplace_back(v1, v2);
      }
    }
    sm1->remove_property_map(tmap1);
    sm2->remove_property_map(tmap2);
  }
};

}  // namespace pyCGAL::wrap::utils
