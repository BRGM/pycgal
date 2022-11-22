#pragma once

#include <CGAL/Polygon_mesh_processing/clip.h>

namespace pyCGAL {

namespace wrap {

namespace Polygon_mesh_processing {

namespace detail {

// this is just a copy of
// CGAL::Polygon_mesh_processing::internal::split_along_edges that will preserve
// constrained edges property

template <class TriangleMesh, class Ecm, class VPM>
void split_along_edges(TriangleMesh& tm, Ecm ecm, VPM vpm) {
  typedef boost::graph_traits<TriangleMesh> GT;
  typedef typename GT::face_descriptor face_descriptor;
  typedef typename GT::edge_descriptor edge_descriptor;
  typedef typename GT::vertex_descriptor vertex_descriptor;
  typedef typename GT::halfedge_descriptor halfedge_descriptor;

  std::vector<edge_descriptor> shared_edges;
  for (edge_descriptor e : edges(tm))
    if (get(ecm, e)) shared_edges.push_back(e);

  std::size_t nb_shared_edges = shared_edges.size();
  std::vector<halfedge_descriptor> hedges_to_update;

  typedef CGAL::dynamic_halfedge_property_t<bool> H_tag;
  typename boost::property_map<TriangleMesh, H_tag>::type no_target_update =
      get(H_tag(), tm);

  std::vector<std::pair<halfedge_descriptor, vertex_descriptor> >
      vertices_to_duplicate;

  // collect border halfedges having as target one of the edge endpoints
  std::set<halfedge_descriptor> extra_border_hedges;
  for (std::size_t k = 0; k < nb_shared_edges; ++k) {
    if (is_border(shared_edges[k], tm)) continue;
    for (halfedge_descriptor h :
         halfedges_around_target(target(shared_edges[k], tm), tm))
      if (is_border(h, tm)) extra_border_hedges.insert(h);
    for (halfedge_descriptor h :
         halfedges_around_target(source(shared_edges[k], tm), tm))
      if (is_border(h, tm)) extra_border_hedges.insert(h);
  }

  for (halfedge_descriptor h : extra_border_hedges) {
    put(no_target_update, h, true);
    set_halfedge(target(h, tm), h, tm);
    hedges_to_update.push_back(h);
  }

  // now duplicate the edge and set its pointers
  for (std::size_t k = 0; k < nb_shared_edges; ++k) {
    if (is_border(shared_edges[k], tm)) continue;
    halfedge_descriptor h = halfedge(shared_edges[k], tm);
    face_descriptor fh = face(h, tm);
    // add edge
    edge_descriptor new_edge = add_edge(tm);
    halfedge_descriptor new_hedge = halfedge(new_edge, tm),
                        new_opp = opposite(new_hedge, tm);
    vertex_descriptor vt = target(h, tm);
    vertex_descriptor vs = source(h, tm);
    // keep track of the constraint property
    ecm[new_edge] = true;

    // replace h with new_hedge
    set_next(new_hedge, next(h, tm), tm);
    set_next(prev(h, tm), new_hedge, tm);
    set_face(new_hedge, fh, tm);
    set_halfedge(fh, new_hedge, tm);

    set_target(new_hedge, vt, tm);
    set_target(new_opp, vs, tm);

    set_face(new_opp, GT::null_face(), tm);
    set_face(h, GT::null_face(), tm);

    // handle vertices to duplicate
    halfedge_descriptor h_vt = halfedge(vt, tm);
    if (get(no_target_update, h_vt))
      vertices_to_duplicate.push_back(std::make_pair(h, vt));
    else
      set_halfedge(vt, h, tm);
    halfedge_descriptor h_vs = halfedge(vs, tm);
    if (get(no_target_update, h_vs))
      vertices_to_duplicate.push_back(std::make_pair(new_opp, vs));
    else
      set_halfedge(vs, new_opp, tm);

    hedges_to_update.push_back(h);
    put(no_target_update, h, true);
    hedges_to_update.push_back(new_opp);
    put(no_target_update, new_opp, true);

    CGAL_assertion(next(prev(new_hedge, tm), tm) == new_hedge);
    CGAL_assertion(prev(next(new_hedge, tm), tm) == new_hedge);
  }

  // update next/prev relationship
  for (halfedge_descriptor h : hedges_to_update) {
    CGAL_assertion(is_border(h, tm));
    halfedge_descriptor h_opp = opposite(h, tm);

    // set next pointer of h, visiting faces inside the patch we consider
    halfedge_descriptor candidate = opposite(prev(h_opp, tm), tm);
    while (!is_border(candidate, tm))
      candidate = opposite(prev(candidate, tm), tm);
    set_next(h, candidate, tm);
    CGAL_assertion(prev(next(h_opp, tm), tm) == h_opp);

    CGAL_assertion(prev(next(h, tm), tm) == h);
    CGAL_assertion(is_border(next(h, tm), tm));
  }

  for (const std::pair<halfedge_descriptor, vertex_descriptor>& p :
       vertices_to_duplicate) {
    vertex_descriptor nv = add_vertex(tm);
    put(vpm, nv, get(vpm, p.second));
    for (halfedge_descriptor h : halfedges_around_target(p.first, tm))
      set_target(h, nv, tm);
    set_halfedge(nv, p.first, tm);
  }

  CGAL_assertion_code(for (halfedge_descriptor h : hedges_to_update)) {
    CGAL_assertion(next(prev(h, tm), tm) == h);
    CGAL_assertion(prev(next(h, tm), tm) == h);
  }

  for (halfedge_descriptor h : hedges_to_update) {
    for (halfedge_descriptor hh : halfedges_around_target(h, tm))
      if (h != hh) set_target(hh, target(h, tm), tm);
  }

  CGAL_assertion(is_valid_polygon_mesh(tm));
}

}  // namespace detail
}  // namespace Polygon_mesh_processing
}  // namespace wrap
}  // namespace pyCGAL
