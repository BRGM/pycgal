#pragma once

#include <pyCGAL/wrap/utils/CGAL_FT.h>
#include <pybind11/numpy.h>

#include <tuple>
#include <type_traits>

#include "Face_connectivity.h"
namespace py = pybind11;

namespace pyCGAL::wrap::utils {

template <typename Surface_mesh>
auto remap_vertices(const Surface_mesh& mesh) {
  using Surface_mesh_index = typename Surface_mesh::size_type;
  const std::size_t ntv =
      mesh.number_of_vertices() + mesh.number_of_removed_vertices();
  std::vector<Surface_mesh_index> vmap(ntv, Surface_mesh::null_vertex());
  Surface_mesh_index k = 0;
  for (auto&& v : mesh.vertices()) {
    vmap[v] = k;
    ++k;
  }
  return vmap;
}

template <typename Surface_mesh>
auto count_faces_by_degree(const Surface_mesh& mesh) {
  typedef typename Surface_mesh::size_type size_type;
  std::vector<size_type> nb_faces;
  for (auto&& f : mesh.faces()) {
    assert(f != Surface_mesh::null_face());
    const size_type d = mesh.degree(f);
    while (nb_faces.size() <= d) nb_faces.push_back(0);
    ++nb_faces[d];
  }
  return nb_faces;
}

template <typename Surface_mesh>
auto collect_vertices_array(const Surface_mesh& mesh) {
  using Point = typename Surface_mesh::Point;
  using FT = CGAL_FT<Point>;
  const std::size_t nv = mesh.number_of_vertices();
  constexpr std::size_t dim = CGAL::Ambient_dimension<Point>::value;
  auto vertices = py::array_t<FT, py::array::c_style>{{nv, dim}};
  static_assert(sizeof(Point) == dim * sizeof(FT), "Inconsistent sizes");
  typedef std::array<FT, dim> Point_as_array;
  static_assert(sizeof(Point) == sizeof(Point_as_array), "Inconsistent sizes");
  auto pv = reinterpret_cast<Point_as_array*>(vertices.mutable_data(0, 0));
  for (auto&& v : mesh.vertices()) {
    (*pv) = *(reinterpret_cast<const Point_as_array*>(&(mesh.point(v))));
    ++pv;
  }
  return vertices;
}

template <typename Category_sizes>
void _check_breaking_order(const Category_sizes& nb_faces_by_degree) {
  int nb_types = 0;
  for (auto&& n : nb_faces_by_degree) {
    if (n > 0) ++nb_types;
  }
  assert(nb_types > 0);
  // this test is not sufficient but it is more than highly probable
  // that the order will be broken in such a situation
  if (nb_types > 1) {
    throw std::runtime_error("Order of Surface_mesh cells will be broken!");
  }
}

template <typename Surface_mesh, typename Category_sizes>
auto collect_connectivity_tables(const Surface_mesh& mesh,
                                 const Category_sizes& nb_faces_by_degree) {
  using Surface_mesh_index = typename Surface_mesh::size_type;
  using Vertex_index = typename Surface_mesh::Vertex_index;
  py::list faces;
  const auto nb_categories = nb_faces_by_degree.size();
  std::vector<Surface_mesh_index*> pointers(nb_categories, nullptr);
  for (std::size_t k = 0; k != nb_categories; ++k) {
    if (nb_faces_by_degree[k] != 0) {
      auto a = Face_connectivity<Surface_mesh>{
          {static_cast<py::ssize_t>(nb_faces_by_degree[k]),
           static_cast<py::ssize_t>(k)}};
      pointers[k] = a.mutable_data(0, 0);
      // we must keep a reference to the array a in a python object
      // so that it's not garbage collected
      faces.append(a);
    }
  }
  const auto vmap = remap_vertices(mesh);
  for (auto&& f : mesh.faces()) {
    for (Vertex_index v : CGAL::vertices_around_face(mesh.halfedge(f), mesh)) {
      auto pf = &(pointers[mesh.degree(f)]);
      (**pf) = vmap[v];
      ++(*pf);
    }
  }
  return faces;
}

template <typename Surface_mesh, typename Category_sizes>
auto collect_face_index_tables(const Surface_mesh& mesh,
                               const Category_sizes& nb_faces_by_degree) {
  using Surface_mesh_index = typename Surface_mesh::size_type;
  py::list indices;
  const auto nb_categories = nb_faces_by_degree.size();
  std::vector<Surface_mesh_index*> pointers(nb_categories, nullptr);
  for (std::size_t k = 0; k != nb_categories; ++k) {
    if (nb_faces_by_degree[k] != 0) {
      auto a = py::array_t<Surface_mesh_index, py::array::c_style>{
          static_cast<py::ssize_t>(nb_faces_by_degree[k])};
      pointers[k] = a.mutable_data(0);
      // we must keep a reference to the array a in a python object
      // so that it's not garbage collected
      indices.append(a);
    }
  }
  Surface_mesh_index k = 0;
  for (auto&& f : mesh.faces()) {
    auto pi = &(pointers[mesh.degree(f)]);
    (**pi) = k;
    ++(*pi);
    ++k;
  }
  return indices;
}

template <bool return_index = false, typename Surface_mesh>
auto collect_faces_arrays(const Surface_mesh& mesh,
                          const bool throw_if_breaking_order = false)
    -> std::conditional_t<return_index, std::tuple<py::list, py::list>,
                          py::list> {
  using Vertex_index = typename Surface_mesh::Vertex_index;
  auto nb_faces_by_degree = count_faces_by_degree(mesh);
  if (throw_if_breaking_order) _check_breaking_order(nb_faces_by_degree);
  if constexpr (return_index) {
    return std::make_tuple(
        collect_connectivity_tables(mesh, nb_faces_by_degree),
        collect_face_index_tables(mesh, nb_faces_by_degree));
  } else {
    return collect_connectivity_tables(mesh, nb_faces_by_degree);
  }
}

template <typename Surface_mesh>
auto as_arrays(const Surface_mesh& mesh, const bool return_index = false,
               const bool throw_if_breaking_order = false) -> py::tuple {
  if (return_index) {
    auto [faces, indices] =
        collect_faces_arrays<true>(mesh, throw_if_breaking_order);
    return py::make_tuple(collect_vertices_array(mesh), faces, indices);
  }
  return py::make_tuple(collect_vertices_array(mesh),
                        collect_faces_arrays(mesh, throw_if_breaking_order));
}

template <typename Surface_mesh>
auto collect_vertices_list(const Surface_mesh& mesh) {
  py::list vertices;
  for (auto&& v : mesh.vertices()) {
    const auto& P = mesh.point(v);
    vertices.append(py::make_tuple(P[0], P[1], P[2]));
  }
  return vertices;
}

template <typename Index, typename Surface_mesh>
auto number_of_valid_indices(const Surface_mesh& mesh) ->
    typename Surface_mesh::size_type {
  if constexpr (std::is_same_v<Index, typename Surface_mesh::Vertex_index>) {
    return mesh.number_of_vertices();
  }
  if constexpr (std::is_same_v<Index, typename Surface_mesh::Halfedge_index>) {
    return mesh.number_of_halfedges();
  }
  if constexpr (std::is_same_v<Index, typename Surface_mesh::Edge_index>) {
    return mesh.number_of_edges();
  }
  if constexpr (std::is_same_v<Index, typename Surface_mesh::Face_index>) {
    return mesh.number_of_faces();
  }
  assert(false);
  return 0;
}

template <typename Index, typename Surface_mesh>
auto valid_indices(const Surface_mesh& mesh) {
  if constexpr (std::is_same_v<Index, typename Surface_mesh::Vertex_index>) {
    return mesh.vertices();
  }
  if constexpr (std::is_same_v<Index, typename Surface_mesh::Halfedge_index>) {
    return mesh.halfedges();
  }
  if constexpr (std::is_same_v<Index, typename Surface_mesh::Edge_index>) {
    return mesh.edges();
  }
  if constexpr (std::is_same_v<Index, typename Surface_mesh::Face_index>) {
    return mesh.faces();
  }
  assert(false);
}

template <typename... Ts>
struct Collect_property;

template <typename T, typename... Ts>
struct Collect_property<T, Ts...> {
  template <typename Index, typename Surface_mesh>
  static py::object search(const Surface_mesh& mesh, const std::string& name) {
    auto [map, exists] = mesh.template property_map<Index, T>(name);
    if (!exists)
      return Collect_property<Ts...>::template search<Index>(mesh, name);
    const py::ssize_t n = number_of_valid_indices<Index>(mesh);
    auto a = py::array_t<T, py::array::c_style>{n};
    auto p = a.mutable_data(0);
    for (auto&& i : valid_indices<Index>(mesh)) {
      *p = map[i];
      ++p;
    }
    return a;
  }
};

template <>
struct Collect_property<> {
  template <typename Index, typename Surface_mesh>
  static py::object search(const Surface_mesh& mesh, const std::string& name) {
    return py::none{};
  }
};

template <typename... Ts>
struct Collect_property<std::tuple<Ts...>> {
  template <typename Index, typename Surface_mesh>
  static py::object search(const Surface_mesh& mesh, const std::string& name) {
    return Collect_property<Ts...>::template search<Index>(mesh, name);
  }
};

template <typename Index, typename Surface_mesh>
py::dict collect_properties(const Surface_mesh& mesh) {
  py::dict result;
  for (auto&& name : mesh.template properties<Index>()) {
    auto pmap =
        Collect_property<pyCGAL::available_property_types>::search<Index>(mesh,
                                                                          name);
    if (!pmap.is_none()) result[py::str{name}] = pmap;
  }
  return result;
}

template <typename Surface_mesh>
auto collect_faces_list(const Surface_mesh& mesh) {
  using Vertex_index = typename Surface_mesh::Vertex_index;
  const auto vmap = remap_vertices(mesh);
  py::list faces;
  for (auto&& f : mesh.faces()) {
    py::list cell;
    for (Vertex_index v : CGAL::vertices_around_face(mesh.halfedge(f), mesh)) {
      cell.append(vmap[v]);
    }
    faces.append(cell);
  }

  return faces;
}

template <typename Surface_mesh>
auto as_lists(const Surface_mesh& mesh, const bool return_vertices_properties,
              const bool return_faces_properties) -> py::list {
  py::list result;
  result.append(collect_vertices_list(mesh));
  result.append(collect_faces_list(mesh));
  if (return_vertices_properties)
    result.append(
        collect_properties<typename Surface_mesh::Vertex_index>(mesh));
  if (return_faces_properties)
    result.append(collect_properties<typename Surface_mesh::Face_index>(mesh));
  return result;
}

}  // namespace pyCGAL::wrap::utils
