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
auto select_indices(
    const Surface_mesh& mesh,
    const typename Surface_mesh::template Property_map<Index, T>& property,
    const TT value) -> std::vector<Index> {
  std::vector<Index> selection;
  for (auto&& i : detail::get<Surface_mesh, Index>::elements(mesh)) {
    if (property[i] == value) selection.emplace_back(i);
  }
  return selection;
}

// T must be comparable to TT
// value is locally copied (assumed to be faster than passing by reference)
template <typename Surface_mesh, typename Index, typename T, typename TT = T>
void keep_indices(
    const Surface_mesh& mesh, std::vector<Index>& selection,
    const typename Surface_mesh::template Property_map<Index, T>& property,
    const TT& value) {
  auto past_kept = std::remove_if(
      begin(selection), end(selection),
      [&property, value](auto&& i) { return property[i] != value; });
  selection.erase(past_kept, end(selection));
}

// T must be comparable to TT
// value is locally copied (assumed to be faster than passing by reference)
template <typename Surface_mesh, typename Index, typename T, typename TT = T>
auto extract_indices(
    const Surface_mesh& mesh, const std::vector<Index>& selection,
    const typename Surface_mesh::template Property_map<Index, T>& property,
    const TT& value) -> std::vector<Index> {
  std::vector<Index> extraction;
  auto past_kept = std::copy_if(
      begin(selection), end(selection), back_inserter(extraction),
      [&property, value](auto&& i) { return property[i] == value; });
  return extraction;
}

// T must be comparable to TT
// value is locally copied (assumed to be faster than passing by reference)
template <typename Surface_mesh, typename T, typename TT = T>
auto select_edge_indices(
    const Surface_mesh& mesh,
    const typename Surface_mesh::template Property_map<
        typename Surface_mesh::Vertex_index, T>& vertex_property,
    const TT value) -> std::vector<typename Surface_mesh::Edge_index> {
  std::vector<typename Surface_mesh::Edge_index> selection;
  for (auto&& e : mesh.edges()) {
    if (vertex_property[mesh.vertex(e, 0)] == value &&
        vertex_property[mesh.vertex(e, 1)] == value) {
      selection.emplace_back(e);
    }
  }
  return selection;
}

template <typename Surface_mesh, typename Property_map>
auto select_crossing_edge_indices(const Surface_mesh& mesh,
                                  const Property_map& vertex_property)
    -> std::vector<typename Surface_mesh::Edge_index> {
  std::vector<typename Surface_mesh::Edge_index> selection;
  for (auto&& e : mesh.edges()) {
    if (vertex_property[mesh.vertex(e, 0)] *
            vertex_property[mesh.vertex(e, 1)] <
        0) {
      selection.emplace_back(e);
    }
  }
  return selection;
}

template <typename Surface_mesh, typename Property_map, typename T>
auto select_crossing_isovalue_edge_indices(const Surface_mesh& mesh,
                                           const Property_map& vertex_property,
                                           const T value)
    -> std::vector<typename Surface_mesh::Edge_index> {
  std::vector<typename Surface_mesh::Edge_index> selection;
  for (auto&& e : mesh.edges()) {
    if ((vertex_property[mesh.vertex(e, 0)] - value) *
            (vertex_property[mesh.vertex(e, 1)] - value) <
        0) {
      selection.emplace_back(e);
    }
  }
  return selection;
}

template <typename Surface_mesh, typename Property_map>
void keep_crossing_edge_indices(
    const Surface_mesh& mesh,
    std::vector<typename Surface_mesh::Edge_index>& selection,
    const Property_map& vertex_property) {
  auto past_kept = std::remove_if(
      begin(selection), end(selection), [&mesh, &vertex_property](auto&& e) {
        return vertex_property[mesh.vertex(e, 0)] *
                   vertex_property[mesh.vertex(e, 1)] >=
               0;
      });
  selection.erase(past_kept, end(selection));
}

template <typename Surface_mesh, typename Property_map, typename T>
void keep_crossing_isovalue_edge_indices(
    const Surface_mesh& mesh,
    std::vector<typename Surface_mesh::Edge_index>& selection,
    const Property_map& vertex_property, const T& value) {
  auto past_kept = std::remove_if(
      begin(selection), end(selection),
      [&mesh, &vertex_property, value](auto&& e) {
        return (vertex_property[mesh.vertex(e, 0)] - value) *
                   (vertex_property[mesh.vertex(e, 1)] - value) >=
               0;
      });
  selection.erase(past_kept, end(selection));
}

}  // namespace pyCGAL::wrap::utils
