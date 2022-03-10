#pragma once

#include <vector>

namespace pyCGAL::wrap::utils {

namespace detail {

template <typename Surface_mesh, typename Index>
struct get;

template <typename Surface_mesh>
struct get<Surface_mesh, typename Surface_mesh::Vertex_index> {
  static auto elements(const Surface_mesh& mesh) { return mesh.vertices(); }
};

template <typename Surface_mesh>
struct get<Surface_mesh, typename Surface_mesh::Face_index> {
  static auto elements(const Surface_mesh& mesh) { return mesh.faces(); }
};

template <typename Surface_mesh>
struct get<Surface_mesh, typename Surface_mesh::Halfedge_index> {
  static auto elements(const Surface_mesh& mesh) { return mesh.halfedges(); }
};

template <typename Surface_mesh>
struct get<Surface_mesh, typename Surface_mesh::Edge_index> {
  static auto elements(const Surface_mesh& mesh) { return mesh.edges(); }
};

}  // namespace detail

// T must be comparable to TT
// value is locally copied (assumed to be faster than passing by reference)
template <typename Surface_mesh, typename Index, typename T, typename TT = T>
auto select(
    const Surface_mesh& mesh,
    const typename Surface_mesh::template Property_map<Index, T>& property,
    const TT value) -> std::vector<Index> {
  std::vector<Index> selection;
  for (auto&& i : detail::get<Surface_mesh, Index>::elements(mesh)) {
    if (property[i] == value) selection.emplace_back(i);
  }
  return selection;
}

}  // namespace pyCGAL::wrap::utils
