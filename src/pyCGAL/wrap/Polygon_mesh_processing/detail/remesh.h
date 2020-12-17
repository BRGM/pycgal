#pragma once

namespace pyCGAL::wrap::Polygon_mesh_processing::detail {

template <typename PolygonMesh,
          typename EdgeRange = typename PolygonMesh::Edge_range,
          typename FaceRange = typename PolygonMesh::Face_range>
struct remesh {};

}  // namespace pyCGAL::wrap::Polygon_mesh_processing::detail
