#pragma once

namespace pyCGAL::wrap::Polygon_mesh_processing::detail {

template <typename PolygonMesh,
          typename FaceRange = typename PolygonMesh::Face_range>
struct reverse_face_orientations {};

}  // namespace pyCGAL::wrap::Polygon_mesh_processing::detail
