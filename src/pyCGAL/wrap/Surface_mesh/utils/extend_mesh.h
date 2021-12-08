#pragma once

#include <pyCGAL/wrap/utils/Coordinates_array.h>

#include <algorithm>
#include <stdexcept>

#include "Face_connectivity.h"
#include "add_vertices.h"

namespace pyCGAL::wrap::utils {

template <typename Surface_mesh>
void add_homogeneous_faces(Surface_mesh& mesh, py::buffer faces,
                           const Vertex_map<Surface_mesh>& vmap,
                           const bool reverse_on_failure = false,
                           const bool throw_on_failure = false) {
  const auto faces_buffer = faces.request();
  typedef typename Surface_mesh::size_type size_type;
  typedef typename Surface_mesh::Vertex_index Vertex_index;
  assert(faces_buffer.shape.size() == 2);
  const auto face_degree = faces_buffer.shape[1];
  if (faces_buffer.strides[0] != face_degree * sizeof(size_type))
    throw std::runtime_error("Inconsistent stride for face offset!");
  if (faces_buffer.strides[1] != sizeof(size_type))
    throw std::runtime_error("Inconsistent stride for vertex index!");
  auto pf = reinterpret_cast<const size_type*>(faces_buffer.ptr);
  const auto end_faces = pf + faces_buffer.shape[0] * face_degree;
  std::vector<Vertex_index> face_vertices(face_degree,
                                          Surface_mesh::null_vertex());
  for (; pf != end_faces; pf += face_degree) {
    face_vertices.clear();
    std::transform(pf, pf + face_degree, std::back_inserter(face_vertices),
                   [&vmap](size_type v) { return vmap.at(v); });
    auto f = mesh.add_face(face_vertices);
    if (reverse_on_failure && f == Surface_mesh::null_face())
      f = mesh.add_face(
          CGAL::make_range(rbegin(face_vertices), rend(face_vertices)));
    if (throw_on_failure && f == Surface_mesh::null_face())
      throw std::runtime_error("Could not insert face!");
  }
}

template <typename Surface_mesh>
void extend_mesh(
    Surface_mesh& mesh,
    utils::Coordinates_array<typename Surface_mesh::Point>& vertices,
    py::list& all_faces, const bool reverse_on_failure = false,
    const bool throw_on_failure = true) {
  const auto vmap = add_vertices(mesh, vertices);
  for (auto&& faces : all_faces)
    add_homogeneous_faces(mesh, faces.cast<Face_connectivity<Surface_mesh>>(),
                          vmap, reverse_on_failure, throw_on_failure);
}

}  // namespace pyCGAL::wrap::utils
