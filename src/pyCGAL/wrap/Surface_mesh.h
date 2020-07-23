#pragma once

#include <CGAL/Surface_mesh.h>
#include <pyCGAL/typedefs.h>

#include "utils/Surface_mesh.h"
#include "utils/wrap_index.h"
#include "utils/wrap_range.h"

namespace pyCGAL {

template <typename Point>
typename WrapTraits<CGAL::Surface_mesh<Point>>::py_class wrap_class(
    WrapTraits<CGAL::Surface_mesh<Point>>, py::module& module) {
  namespace wutils = wrap::utils;
  namespace SMU = wutils::Surface_mesh;

  using Wrap = WrapTraits<CGAL::Surface_mesh<Point>>;
  using Surface_mesh = typename Wrap::cpp_type;
  using Surface_mesh_index = typename Surface_mesh::size_type;
  using Vertex_index = typename Surface_mesh::Vertex_index;
  using Halfedge_index = typename Surface_mesh::Halfedge_index;
  using Edge_index = typename Surface_mesh::Edge_index;
  using Face_index = typename Surface_mesh::Face_index;
  using Vertex_range = typename Surface_mesh::Vertex_range;
  using Halfedge_range = typename Surface_mesh::Halfedge_range;
  using Edge_range = typename Surface_mesh::Edge_range;
  using Face_range = typename Surface_mesh::Face_range;

  wutils::wrap_index<Vertex_index, Surface_mesh_index>(module, "Vertex_index");
  wutils::wrap_index<Halfedge_index, Surface_mesh_index>(module,
                                                         "Halfedge_index");
  wutils::wrap_index<Edge_index, Surface_mesh_index>(module, "Edge_index");
  wutils::wrap_index<Face_index, Surface_mesh_index>(module, "Face_index");

  wutils::wrap_range<Vertex_range>(module, "Vertex_range");
  wutils::wrap_range<Halfedge_range>(module, "Halfedge_range");
  wutils::wrap_range<Edge_range>(module, "Edge_range");
  wutils::wrap_range<Face_range>(module, "Face_range");

  // FIXME: Weirdly enough trying to reserve memory in init methods using
  //        Surface_mesh::reserve generates
  //        Memory Error Bad allocation at run time for big meshes
  //        depending on the python version... Is it linked to direct
  //        contiguous allocation on the heap?
  typename WrapTraits<CGAL::Surface_mesh<Point>>::py_class pyclass =
      py::class_<Surface_mesh>(module, "Surface_mesh");
  pyclass.def(py::init<>());
  pyclass.def(py::init<const Surface_mesh&>());
  pyclass.def(py::init([](wutils::Coordinates_array<Point>& vertices,
                          SMU::Face_connectivity<Surface_mesh>& faces) {
    return SMU::make_mesh<Surface_mesh>(vertices, py::make_tuple(faces));
  }));
  pyclass.def(py::init(&SMU::make_mesh<Surface_mesh>));

  pyclass.def("number_of_vertices", &Surface_mesh::number_of_vertices);
  pyclass.def("number_of_halfedges", &Surface_mesh::number_of_halfedges);
  pyclass.def("number_of_edges", &Surface_mesh::number_of_edges);
  pyclass.def("number_of_faces", &Surface_mesh::number_of_faces);
  pyclass.def("is_empty", &Surface_mesh::is_empty);
  pyclass.def("join", &Surface_mesh::join);
  pyclass.def("vertices", &Surface_mesh::vertices);
  pyclass.def("halfedges", &Surface_mesh::halfedges);
  pyclass.def("edges", &Surface_mesh::edges);
  pyclass.def("faces", &Surface_mesh::faces);
  pyclass.def("point", py::overload_cast<Vertex_index>(&Surface_mesh::point,
                                                       py::const_));
  pyclass.def("degree", py::overload_cast<Vertex_index>(&Surface_mesh::degree,
                                                        py::const_));
  pyclass.def("degree",
              py::overload_cast<Face_index>(&Surface_mesh::degree, py::const_));

  pyclass.def("extend", &SMU::extend_mesh<Surface_mesh>);
  pyclass.def("as_arrays", &SMU::as_arrays<Surface_mesh>);

  return pyclass;
}

}  // namespace pyCGAL
