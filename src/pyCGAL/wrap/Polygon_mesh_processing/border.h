#pragma once

#include <CGAL/Kernel/global_functions.h>
#include <CGAL/Polygon_mesh_processing/border.h>
#include <pyCGAL/typedefs.h>

#include <stdexcept>
#include <vector>

#include "detail/border.h"

namespace pyCGAL {

namespace {

template <typename PolygonMesh>
py::tuple extract_zmap_corners_and_borders(PolygonMesh& mesh,
                                           const double limit_angle) {
  using Point = typename PolygonMesh::Point;
  using Halfedge_index = typename PolygonMesh::Halfedge_index;
  using Vertex_index = typename PolygonMesh::Vertex_index;
  using Edge_index = typename PolygonMesh::Edge_index;

  auto project = [&mesh](const Vertex_index v) {
    const Point& P = mesh.point(v);
    return Point{P.x(), P.y(), 0};
  };

  std::vector<Halfedge_index> seeds;
  CGAL::Polygon_mesh_processing::extract_boundary_cycles(mesh,
                                                         back_inserter(seeds));

  std::vector<Vertex_index> corners;
  std::vector<std::vector<Edge_index>> edges;
  for (auto&& seed : seeds) {
    auto h0 = seed;
    auto h1 = mesh.next(h0);
    auto p = project(mesh.source(h0));
    auto q = project(mesh.target(h0));
    h1 = mesh.next(h0);
    while (h1 != seed) {
      assert(mesh.target(h0) == mesh.source(h1));
      auto r = project(mesh.target(h1));
      if (CGAL::approximate_angle(p, q, r) < limit_angle) {
        corners.emplace_back(mesh.target(h0));
        break;
      }
      h0 = h1;
      h1 = mesh.next(h0);
      p = q;
      q = r;
    }
    const auto start = h1;
    h0 = start;
    edges.emplace_back();
    edges.back().emplace_back(mesh.edge(h0));
    p = project(mesh.source(h0));
    q = project(mesh.target(h0));
    h1 = mesh.next(h0);
    while (h1 != start) {
      assert(mesh.target(h0) == mesh.source(h1));
      auto r = project(mesh.target(h1));
      if (CGAL::approximate_angle(p, q, r) < limit_angle) {
        corners.emplace_back(mesh.target(h0));
        edges.emplace_back();
      }
      edges.back().emplace_back(mesh.edge(h1));
      h0 = h1;
      h1 = mesh.next(h0);
      p = q;
      q = r;
    }
  }
  // CHECKME: avoid the copy ?
  py::list result;
  for (auto&& part : edges) {
    result.append(part);
  }
  return py::make_tuple(corners, result);
}

}  // namespace

namespace detail = pyCGAL::wrap::Polygon_mesh_processing::detail;

template <typename PolygonMesh, typename Halfedge_descriptor,
          typename FaceRange>
void wrap_element(detail::border<PolygonMesh, Halfedge_descriptor, FaceRange>,
                  py::module& module) {
  module.def(
      "border_halfedges",
      [](PolygonMesh& mesh, py::object face_group) {
        const FaceRange& face_range = face_group.is_none()
                                          ? faces(mesh)
                                          : face_group.cast<const FaceRange&>();
        std::vector<Halfedge_descriptor> borders;
        CGAL::Polygon_mesh_processing::border_halfedges(face_range, mesh,
                                                        back_inserter(borders));
        return borders;
      },
      py::arg("mesh").none(false), py::arg("face_group") = py::none());

  module.def(
      "extract_boundary_cycles",
      [](PolygonMesh& mesh) {
        std::vector<Halfedge_descriptor> borders;
        CGAL::Polygon_mesh_processing::extract_boundary_cycles(
            mesh, back_inserter(borders));
        return borders;
      },
      py::arg("mesh").none(false));

  module.def("extract_zmap_corners_and_borders",
             &extract_zmap_corners_and_borders<PolygonMesh>,
             py::arg("mesh").none(false), py::arg("limit_angle") = 180.);
}

}  // namespace pyCGAL
