#pragma once

namespace pyCGAL::wrap::Polygon_mesh_processing::detail {

template <typename PolygonMesh,
          typename FaceRange = typename PolygonMesh::Face_range>
struct triangulate_faces {};

}  // namespace pyCGAL::wrap::Polygon_mesh_processing::detail
