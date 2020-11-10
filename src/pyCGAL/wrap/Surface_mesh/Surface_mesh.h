#pragma once

#include <CGAL/Surface_mesh.h>
#include <pyCGAL/typedefs.h>
#include <pyCGAL/wrap/utils/wrap_index.h>
#include <pyCGAL/wrap/utils/wrap_range.h>

#include "utils/Surface_mesh.h"

namespace pyCGAL {

template <typename Point>
typename WrapTraits<CGAL::Surface_mesh<Point>>::py_class wrap_class(
    WrapTraits<CGAL::Surface_mesh<Point>>, py::module& module) {
  namespace wutils = wrap::utils;

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
  typename Wrap::py_class pyclass =
      py::class_<Surface_mesh>(module, "Surface_mesh");
  pyclass.def(py::init<>());
  pyclass.def(py::init<const Surface_mesh&>());
  pyclass.def(py::init([](wutils::Coordinates_array<Point>& vertices,
                          wutils::Face_connectivity<Surface_mesh>& faces) {
    return wutils::make_mesh<Surface_mesh>(vertices, py::make_tuple(faces));
  }));
  pyclass.def(py::init(&wutils::make_mesh<Surface_mesh>));

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
  pyclass.def("remove_vertex", &Surface_mesh::remove_vertex);
  pyclass.def("remove_edge", &Surface_mesh::remove_edge);
  pyclass.def("remove_face", &Surface_mesh::remove_face);

  pyclass.def("has_garbage", &Surface_mesh::has_garbage);
  pyclass.def("collect_garbage",
              (void (Surface_mesh::*)()) & Surface_mesh::collect_garbage);

  pyclass.def("extend", &wutils::extend_mesh<Surface_mesh>, py::arg("vertices"),
              py::arg("all_faces"), py::arg("reverse_on_failure") = false,
              py::arg("throw_on_failure") = true);
  pyclass.def("centroids", &wutils::centroids<Surface_mesh>);
  pyclass.def("as_arrays", &wutils::as_arrays<Surface_mesh>);

  pyclass.def("read_off", &wutils::read_off<Point>);
  pyclass.def("write_off", &wutils::write_off<Point>);
  pyclass.def_static("from_off", [](const std::string& filename) {
    auto p = std::make_unique<Surface_mesh>();
    wutils::read_off(*p, filename);
    return p;
  });

  wutils::wrap_property_map<Vertex_index>(module, pyclass, "vertex");
  wutils::wrap_property_map<Halfedge_index>(module, pyclass, "halfedge");
  wutils::wrap_property_map<Edge_index>(module, pyclass, "edge");
  wutils::wrap_property_map<Face_index>(module, pyclass, "face");

  return pyclass;
}

}  // namespace pyCGAL
