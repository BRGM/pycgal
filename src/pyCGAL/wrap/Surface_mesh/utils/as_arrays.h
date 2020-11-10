#pragma once

#include <pyCGAL/wrap/utils/CGAL_FT.h>
#include <pybind11/numpy.h>

#include <tuple>

#include "Face_connectivity.h"
namespace py = pybind11;

namespace pyCGAL::wrap::utils {

template <typename Surface_mesh>
auto remap_vertices(const Surface_mesh& mesh) {
  typedef typename Surface_mesh::Point Point;
  typedef typename Surface_mesh::size_type Surface_mesh_index;
  typedef typename Surface_mesh::Vertex_index Vertex_index;
  typedef CGAL_FT<Point> FT;
  const std::size_t nv = mesh.number_of_vertices();
  constexpr std::size_t dim = CGAL::Ambient_dimension<Point>::value;
  auto va = py::array_t<FT, py::array::c_style>{{nv, dim}};
  static_assert(sizeof(Point) == dim * sizeof(FT), "Inconsistent sizes");
  typedef std::array<FT, dim> Point_as_array;
  static_assert(sizeof(Point) == sizeof(Point_as_array), "Inconsistent sizes");
  auto pv = reinterpret_cast<Point_as_array*>(va.mutable_data(0, 0));
  const std::size_t ntv = nv + mesh.number_of_removed_vertices();
  std::vector<Surface_mesh_index> vmap(ntv, Surface_mesh::null_vertex());
  Surface_mesh_index k = 0;
  for (auto&& v : mesh.vertices()) {
    (*pv) = *(reinterpret_cast<const Point_as_array*>(&(mesh.point(v))));
    vmap[v] = k;
    ++pv;
    ++k;
  }
  return std::make_tuple(vmap, va);
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
auto as_arrays(const Surface_mesh& mesh) -> py::tuple {
  typedef typename Surface_mesh::Point Point;
  typedef typename Surface_mesh::size_type Surface_mesh_index;
  typedef typename Surface_mesh::Vertex_index Vertex_index;
  typedef CGAL_FT<Point> FT;
  std::vector<Surface_mesh_index> vmap;
  py::array_t<FT, py::array::c_style> vertices;
  std::tie(vmap, vertices) = remap_vertices(mesh);
  auto nb_faces_by_degree = count_faces_by_degree(mesh);
  const auto nb_categories = nb_faces_by_degree.size();
  py::list faces;
  std::vector<Surface_mesh_index*> pointers(nb_categories, nullptr);
  for (std::size_t k = 0; k != nb_categories; ++k) {
    if (nb_faces_by_degree[k] != 0) {
      auto a = Face_connectivity<Surface_mesh>{
          {static_cast<std::size_t>(nb_faces_by_degree[k]),
           static_cast<std::size_t>(k)}};
      pointers[k] = a.mutable_data(0, 0);
      faces.append(a);
    }
  }
  for (auto&& f : mesh.faces()) {
    for (Vertex_index v : CGAL::vertices_around_face(mesh.halfedge(f), mesh)) {
      Surface_mesh_index** pf = &(pointers[mesh.degree(f)]);
      (**pf) = vmap[v];
      ++(*pf);
    }
  }
  return py::make_tuple(vertices, faces);
}

}  // namespace pyCGAL::wrap::utils
