#pragma once

#include <CGAL/Dimension.h>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
namespace py = pybind11;

namespace pyCGAL::wrap::utils {

template <typename Triangulation>
auto as_arrays(const Triangulation& tr) {
  typedef typename Triangulation::Vertex_handle Vertex_handle;
  typedef typename Triangulation::Point Point;

  const std::size_t nv = tr.number_of_vertices();
  constexpr int dim = CGAL::Ambient_dimension<Point>::value;
  auto vertices = py::array_t<double, py::array::c_style>{
      {nv, static_cast<std::size_t>(dim)}};
  auto rv = vertices.mutable_unchecked<2>();
  std::map<Vertex_handle, std::size_t> vmap;
  std::size_t n = 0;
  for (auto v = tr.finite_vertices_begin(); v != tr.finite_vertices_end();
       ++v) {
    vmap[v] = n;
    auto P = v->point();
    for (int k = 0; k < dim; ++k) rv(n, k) = P[k];
    ++n;
  }
  const std::size_t nf = tr.number_of_faces();
  auto triangles = py::array_t<std::size_t, py::array::c_style>{
      {nf, static_cast<std::size_t>(3)}};
  auto rt = triangles.mutable_unchecked<2>();
  n = 0;
  for (auto f = tr.finite_faces_begin(); f != tr.finite_faces_end(); ++f) {
    for (int k = 0; k < 3; ++k) rt(n, k) = vmap[f->vertex(k)];
    ++n;
  }
  return py::make_tuple(vertices, triangles);
}

}  // namespace pyCGAL::wrap::utils
