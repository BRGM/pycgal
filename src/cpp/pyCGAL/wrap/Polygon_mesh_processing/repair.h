#pragma once

#include <CGAL/Polygon_mesh_processing/manifoldness.h>
#include <CGAL/Polygon_mesh_processing/repair.h>
#include <CGAL/Polygon_mesh_processing/self_intersections.h>
#include <CGAL/Polygon_mesh_processing/shape_predicates.h>
#include <pyCGAL/typedefs.h>

#include "detail/repair.h"

namespace pyCGAL {

namespace detail = pyCGAL::wrap::Polygon_mesh_processing::detail;

template <typename TriangleMesh>
void wrap_element(detail::repair<TriangleMesh>, py::module& module) {
  using Vertex_index = typename TriangleMesh::Vertex_index;
  using Halfedge_index = typename TriangleMesh::Halfedge_index;
  using Edge_index = typename TriangleMesh::Edge_index;
  using Face_index = typename TriangleMesh::Face_index;

  module.def(
      "does_self_intersect",
      [](TriangleMesh& mesh) {
        return CGAL::Polygon_mesh_processing::does_self_intersect(mesh);
      },
      py::arg("mesh").none(false));

  module.def(
      "non_manifold_vertices",
      [](TriangleMesh& mesh) {
        std::vector<Halfedge_index> halfedges;
        CGAL::Polygon_mesh_processing::non_manifold_vertices(
            mesh, back_inserter(halfedges));
        return halfedges;
      },
      py::arg("mesh").none(false));

  module.def(
      "degenerate_edges",
      [](TriangleMesh& mesh) {
        std::vector<Edge_index> edges;
        CGAL::Polygon_mesh_processing::degenerate_edges(mesh,
                                                        back_inserter(edges));
        return edges;
      },
      py::arg("mesh").none(false));

  module.def(
      "degenerate_faces",
      [](TriangleMesh& mesh) {
        std::vector<Face_index> faces;
        CGAL::Polygon_mesh_processing::degenerate_faces(mesh,
                                                        back_inserter(faces));
        return faces;
      },
      py::arg("mesh").none(false));

  module.def(
      "remove_isolated_vertices",
      &CGAL::Polygon_mesh_processing::remove_isolated_vertices<TriangleMesh>,
      py::arg("mesh").none(false));

  module.def(
      "self_intersections",
      [](TriangleMesh& mesh) {
        std::vector<std::pair<Face_index, Face_index>> intersecting_faces;
        CGAL::Polygon_mesh_processing::self_intersections(
            mesh, back_inserter(intersecting_faces));
        py::list result;
        for (auto&& faces : intersecting_faces) {
          result.append(py::make_tuple(faces.first, faces.second));
        }
        return result;
      },
      py::arg("mesh").none(false));
}

}  // namespace pyCGAL
