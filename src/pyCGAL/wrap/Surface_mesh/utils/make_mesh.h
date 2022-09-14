#pragma once

#include <pybind11/pybind11.h>
namespace py = pybind11;

#include <pyCGAL/wrap/utils/Coordinates_array.h>

#include "add_vertices.h"
#include "extend_mesh.h"

namespace pyCGAL::wrap::utils {

template <typename Surface_mesh>
auto make_mesh(utils::Coordinates_array<typename Surface_mesh::Point>& vertices,
               py::list faces, Extension_data<Surface_mesh>& data) {
  auto mesh = std::make_unique<Surface_mesh>();
  extend_mesh(*mesh, data, vertices, faces);
  return mesh;
}

template <typename Surface_mesh>
auto make_mesh(utils::Coordinates_array<typename Surface_mesh::Point>& vertices,
               py::list faces) {
  Extension_data<Surface_mesh> data;
  return make_mesh(vertices, faces, data);
}

template <typename Surface_mesh>
auto make_mesh(py::list vertices, py::list faces) {
  using Point = typename Surface_mesh::Point;
  constexpr auto dim = CGAL::Ambient_dimension<Point>::value;
  std::vector<py::ssize_t> shape;
  shape.push_back(py::len(vertices));
  shape.push_back(dim);
  utils::Coordinates_array<Point> a{shape};
  py::ssize_t k = 0;
  for (auto&& P : vertices) {
    assert(k != shape[0]);
    try {
      (*reinterpret_cast<Point*>(a.mutable_data(k, 0))) = P.cast<Point>();
    } catch (py::cast_error) {
      try {
        auto seq = P.cast<py::sequence>();
        if (py::len(seq) != dim) {
          throw std::runtime_error("Wrong point dimension!");
        }
        py::ssize_t l = 0;
        for (auto&& x : seq) {
          (*a.mutable_data(k, l)) = x.cast<CGAL_FT<Point>>();
          ++l;
        }
      } catch (py::cast_error) {
        throw std::runtime_error("Unkown vertices type!");
      }
    }
    ++k;
  }
  py::list homogeneous_faces;
  int nb_sublists = 0;
  for (auto&& face_vertices : faces) {
    const auto nv = py::len(face_vertices);
    assert(nv > 2);
    while (nb_sublists + 2 < nv) {
      homogeneous_faces.append(py::list{});
      nb_sublists++;
    }
    assert(nv - 3 < py::len(homogeneous_faces));
    homogeneous_faces[nv - 3].cast<py::list>().append(face_vertices);
  }
  return make_mesh<Surface_mesh>(a, homogeneous_faces);
}

}  // namespace pyCGAL::wrap::utils
