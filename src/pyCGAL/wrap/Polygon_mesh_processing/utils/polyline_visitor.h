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
  using Edge_index = typename Surface_mesh::Edge_index;
  using Polyline_map =
      typename Surface_mesh::template Property_map<Edge_index, Polyline_id>;
  using Twins = std::pair<Vertex_index, Vertex_index>;
  using Twins_container = std::vector<Twins>;
  const Surface_mesh* sm1 = nullptr;
  const Surface_mesh* sm2 = nullptr;
  Twins_container* ptwins = nullptr;
  // property maps are just pointers underneath: copies are shallow
  std::optional<std::pair<Polyline_map, Polyline_map>> pmaps;
  halfedge_descriptor splitted_edge;
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
    splitted_edge = h;
  }
  void edge_split(halfedge_descriptor hnew, const Surface_mesh& sm) {
    assert(splitted_mesh == &sm);
    if (pmaps) {
      auto& pmap = (&sm == sm1) ? pmaps->first : pmaps->second;
      pmap[sm.edge(hnew)] = pmap[sm.edge(splitted_edge)];
    }
  }
  void after_edge_split() {
#ifndef NDEBUG
    assert(splitted_mesh);
    splitted_mesh = nullptr;
#endif
  }
};

}  // namespace pyCGAL::wrap::utils
