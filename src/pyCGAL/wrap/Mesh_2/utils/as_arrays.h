#pragma once

#include <CGAL/Dimension.h>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
namespace py = pybind11;

namespace pyCGAL::wrap::utils {

template <typename Triangulation>
auto remap_vertices(const Triangulation& tr) {
  typedef typename Triangulation::Vertex_handle Vertex_handle;
  std::map<Vertex_handle, std::size_t> vmap;
  std::size_t n = 0;
  for (auto v = tr.finite_vertices_begin(); v != tr.finite_vertices_end();
       ++v) {
    vmap[v] = n;
    ++n;
  }
  return vmap;
}

template <typename Triangulation>
auto collect_vertices(const Triangulation& tr) {
  using Point = typename Triangulation::Point;

  const std::size_t nv = tr.number_of_vertices();
  constexpr int dim = CGAL::Ambient_dimension<Point>::value;
  auto vertices = py::array_t<double, py::array::c_style>{
      {nv, static_cast<std::size_t>(dim)}};
  auto rv = vertices.mutable_unchecked<2>();
  std::size_t n = 0;
  for (auto v = tr.finite_vertices_begin(); v != tr.finite_vertices_end();
       ++v) {
    const auto P = v->point();
    for (int k = 0; k < dim; ++k) rv(n, k) = P[k];
    ++n;
  }
  return vertices;
}

template <typename Triangulation, typename Vertices_map>
auto collect_faces(const Triangulation& tr, const Vertices_map& vmap) {
  const std::size_t nf = tr.number_of_faces();
  auto triangles = py::array_t<std::size_t, py::array::c_style>{
      {nf, static_cast<std::size_t>(3)}};
  auto rt = triangles.mutable_unchecked<2>();
  std::size_t n = 0;
  for (auto f = tr.finite_faces_begin(); f != tr.finite_faces_end(); ++f) {
    for (int k = 0; k < 3; ++k) rt(n, k) = vmap.at(f->vertex(k));
    ++n;
  }
  return triangles;
}

template <typename Edges, typename Vertices_map>
auto edges_vertices(const Edges& edges, const Vertices_map& vmap) {
  const std::size_t ne = size(edges);
  auto edges_array = py::array_t<std::size_t, py::array::c_style>{
      {ne, static_cast<std::size_t>(2)}};
  auto re = edges_array.mutable_unchecked<2>();
  std::size_t n = 0;
  for (auto&& e : edges) {
    auto [fh, i] = e;
    re(n, 0) = vmap.at(fh->vertex((i + 1) % 3));
    re(n, 1) = vmap.at(fh->vertex((i + 2) % 3));
    ++n;
  }
  return edges_array;
}

template <typename Triangulation>
auto as_arrays(const Triangulation& tr) {
  const auto vmap = remap_vertices(tr);
  return py::make_tuple(collect_vertices(tr), collect_faces(tr, vmap));
}

template <typename Triangulation>
auto as_arrays_with_constraints(const Triangulation& tr) {
  const auto vmap = remap_vertices(tr);
  return py::make_tuple(collect_vertices(tr), collect_faces(tr, vmap),
                        edges_vertices(tr.constrained_edges(), vmap));
}

}  // namespace pyCGAL::wrap::utils
