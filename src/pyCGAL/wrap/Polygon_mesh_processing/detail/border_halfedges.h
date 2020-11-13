#pragma once

namespace pyCGAL::wrap::Polygon_mesh_processing::detail {

template <typename PolygonMesh,
          typename Halfedge_descriptor = typename PolygonMesh::Halfedge_index,
          typename FaceRange = typename PolygonMesh::Face_range>
struct border_halfedges {};

}  // namespace pyCGAL::wrap::Polygon_mesh_processing::detail
