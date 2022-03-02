#pragma once

#include <CGAL/Surface_mesh.h>
#include <pyCGAL/typedefs.h>
#include <pyCGAL/wrap/utils/wrap_index.h>
#include <pyCGAL/wrap/utils/wrap_range.h>
#include <pyCGAL/wrap/utils/wrap_stl_container.h>

#include <memory>

#include "utils/Surface_mesh.h"

namespace pyCGAL {

namespace detail {
template <typename Index, typename Range_type>
auto index_vector_from_range(Range_type& range)
    -> std::unique_ptr<std::vector<Index>> {
  auto v = std::make_unique<std::vector<Index>>();
  assert(v);
  v->insert(v->end(), begin(range), end(range));
  return v;
}
}  // namespace detail

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

  // We deactivate buffer interface to prevent mixing indices with constructors
  auto Vertices_class =
      wutils::wrap_stl_vector<Vertex_index, Surface_mesh_index>(
          module, "Vertices", false, false);
  Vertices_class.def(
      py::init(&detail::index_vector_from_range<Vertex_index, Vertex_range>));
  auto Halfedges_class =
      wutils::wrap_stl_vector<Halfedge_index, Surface_mesh_index>(
          module, "Halfedges", false, false);
  Halfedges_class.def(py::init(
      &detail::index_vector_from_range<Halfedge_index, Halfedge_range>));
  auto Edges_class = wutils::wrap_stl_vector<Edge_index, Surface_mesh_index>(
      module, "Edges", false, false);
  Edges_class.def(
      py::init(&detail::index_vector_from_range<Edge_index, Edge_range>));
  Edges_class.def(py::init([](const Surface_mesh& mesh,
                              const std::vector<Halfedge_index>& halfedges) {
    auto v = std::make_unique<std::vector<Edge_index>>();
    assert(v);
    v->reserve(halfedges.size());
    for (auto&& h : halfedges) {
      v->emplace_back(mesh.edge(h));
    }
    return v;
  }));
  auto Faces_class = wutils::wrap_stl_vector<Face_index, Surface_mesh_index>(
      module, "Faces", false, false);
  Faces_class.def(
      py::init(&detail::index_vector_from_range<Face_index, Face_range>));
  Faces_class.def(py::init([](const Surface_mesh& mesh,
                              const std::vector<Halfedge_index>& halfedges,
                              const bool opposite_faces = false) {
                    auto v = std::make_unique<std::vector<Face_index>>();
                    assert(v);
                    v->reserve(halfedges.size());
                    if (opposite_faces) {
                      for (auto&& h : halfedges) {
                        v->emplace_back(mesh.face(mesh.opposite(h)));
                      }
                    } else {
                      for (auto&& h : halfedges) {
                        v->emplace_back(mesh.face(h));
                      }
                    }
                    return v;
                  }),
                  py::arg("mesh").none(false), py::arg("halfedges").none(false),
                  py::arg("opposite_faces") = false);
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
  pyclass.def(
      py::init(py::overload_cast<wutils::Coordinates_array<Point>&, py::list>(
          wutils::make_mesh<Surface_mesh>)));

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
  pyclass.def("__getitem__", py::overload_cast<Vertex_index>(
                                 &Surface_mesh::point, py::const_));
  pyclass.def("__setitem__", [](Surface_mesh& self, Vertex_index v,
                                const Point& P) { self.point(v) = P; });
  pyclass.def("remove_vertex", &Surface_mesh::remove_vertex);
  pyclass.def("remove_edge", &Surface_mesh::remove_edge);
  pyclass.def("remove_face", &Surface_mesh::remove_face);

  pyclass.def("has_garbage", &Surface_mesh::has_garbage);
  pyclass.def("collect_garbage",
              (void (Surface_mesh::*)()) & Surface_mesh::collect_garbage);

  pyclass.def("__iadd__", [](Surface_mesh& self, const Surface_mesh& other) {
    self += other;
    return self;
  });
  pyclass.def("__add__",
              [](const Surface_mesh& self, const Surface_mesh& other) {
                Surface_mesh copy{self};
                copy += other;
                return copy;
              });
  pyclass.def(
      "extend",
      [](Surface_mesh& self, wutils::Coordinates_array<Point>& vertices,
         py::list& all_faces, const bool reverse_on_failure,
         const bool throw_on_failure) {
        wutils::Extension_data<Surface_mesh> data;
        wutils::extend_mesh<Surface_mesh>(self, data, vertices, all_faces,
                                          reverse_on_failure, throw_on_failure);
      },
      py::arg("vertices"), py::arg("all_faces"),
      py::arg("reverse_on_failure") = false,
      py::arg("throw_on_failure") = true);
  pyclass.def("centroid", &wutils::centroid<Surface_mesh>);
  pyclass.def("centroids", &wutils::centroids<Surface_mesh>);
  pyclass.def("as_arrays", &wutils::as_arrays<Surface_mesh>,
              py::arg("return_index") = false,
              py::arg("throw_on_breaking_order") = false);
  pyclass.def("as_lists", &wutils::as_lists<Surface_mesh>);

  pyclass.def("is_removed", py::overload_cast<Vertex_index>(
                                &Surface_mesh::is_removed, py::const_));
  pyclass.def("is_removed", py::overload_cast<Halfedge_index>(
                                &Surface_mesh::is_removed, py::const_));
  pyclass.def("is_removed", py::overload_cast<Edge_index>(
                                &Surface_mesh::is_removed, py::const_));
  pyclass.def("is_removed", py::overload_cast<Face_index>(
                                &Surface_mesh::is_removed, py::const_));

  pyclass.def("is_valid",
              py::overload_cast<bool>(&Surface_mesh::is_valid, py::const_),
              py::arg("verbose") = true);
  pyclass.def("is_valid", py::overload_cast<Vertex_index>(
                              &Surface_mesh::is_valid, py::const_));
  pyclass.def("is_valid", py::overload_cast<Halfedge_index>(
                              &Surface_mesh::is_valid, py::const_));
  pyclass.def("is_valid", py::overload_cast<Edge_index>(&Surface_mesh::is_valid,
                                                        py::const_));
  pyclass.def("is_valid", py::overload_cast<Face_index>(&Surface_mesh::is_valid,
                                                        py::const_));

  pyclass.def("is_border",
              py::overload_cast<Vertex_index, bool>(&Surface_mesh::is_border,
                                                    py::const_),
              py::arg("v"), py::arg("check_all_incident_halfedges") = true);
  pyclass.def("is_border", py::overload_cast<Halfedge_index>(
                               &Surface_mesh::is_border, py::const_));
  pyclass.def("is_border", py::overload_cast<Edge_index>(
                               &Surface_mesh::is_border, py::const_));

  // Low-Level Connectivity
  pyclass.def("target", &Surface_mesh::target);
  pyclass.def("set_target", &Surface_mesh::set_target);
  pyclass.def("face", &Surface_mesh::face);
  pyclass.def("set_face", &Surface_mesh::set_face);
  pyclass.def("next", &Surface_mesh::next);
  pyclass.def("prev", &Surface_mesh::prev);
  pyclass.def("set_next", &Surface_mesh::set_next);
  pyclass.def("halfedge", py::overload_cast<Vertex_index>(
                              &Surface_mesh::halfedge, py::const_));
  pyclass.def("set_halfedge", py::overload_cast<Vertex_index, Halfedge_index>(
                                  &Surface_mesh::set_halfedge));
  pyclass.def("halfedge", py::overload_cast<Face_index>(&Surface_mesh::halfedge,
                                                        py::const_));
  pyclass.def("set_halfedge", py::overload_cast<Face_index, Halfedge_index>(
                                  &Surface_mesh::set_halfedge));
  pyclass.def("opposite", &Surface_mesh::opposite);

  // Low - Level Connectivity Convenience Functions
  pyclass.def("source", &Surface_mesh::source);
  pyclass.def("next_around_target", &Surface_mesh::next_around_target);
  pyclass.def("prev_around_target", &Surface_mesh::prev_around_target);
  pyclass.def("next_around_source", &Surface_mesh::next_around_source);
  pyclass.def("prev_around_source", &Surface_mesh::prev_around_source);
  pyclass.def("vertex", &Surface_mesh::vertex);
  pyclass.def("halfedge", py::overload_cast<Vertex_index, Vertex_index>(
                              &Surface_mesh::halfedge, py::const_));

  // Switching between Halfedges and Edges
  pyclass.def("edge", &Surface_mesh::edge);
  pyclass.def("edge", [](Surface_mesh& self, const Vertex_index v1,
                         const Vertex_index v2) {
    const auto h = self.halfedge(v1, v2);
    if (h == self.null_halfedge())
      throw std::runtime_error("Non connected vertices.");
    return self.edge(h);
  });

  pyclass.def("halfedge", py::overload_cast<Edge_index>(&Surface_mesh::halfedge,
                                                        py::const_));
  pyclass.def("halfedge", py::overload_cast<Edge_index, unsigned int>(
                              &Surface_mesh::halfedge, py::const_));

  // Degree Functions
  pyclass.def("degree", py::overload_cast<Vertex_index>(&Surface_mesh::degree,
                                                        py::const_));
  pyclass.def("degree",
              py::overload_cast<Face_index>(&Surface_mesh::degree, py::const_));

  // Null Elements
  pyclass.def_property_readonly("null_vertex", [](const Surface_mesh&) {
    return Surface_mesh::null_vertex();
  });
  pyclass.def_property_readonly("null_face", [](const Surface_mesh&) {
    return Surface_mesh::null_face();
  });
  pyclass.def_property_readonly("null_halfedge", [](const Surface_mesh&) {
    return Surface_mesh::null_halfedge();
  });
  pyclass.def_property_readonly("null_edge", [](const Surface_mesh&) {
    return Surface_mesh::null_edge();
  });

  pyclass.def("read_off", &wutils::read_off<Point>);
  pyclass.def("write_off", &wutils::write_off<Point>);
  pyclass.def_static("from_off", [](const std::string& filename) {
    auto p = std::make_unique<Surface_mesh>();
    wutils::read_off(*p, filename);
    return p;
  });

  auto process_mesh_output =
      [](Surface_mesh* pmesh, wutils::Extension_data<Surface_mesh> data,
         bool with_vertices_map, bool with_faces_map) -> py::object {
    auto mesh = py::cast(pmesh);
    if (with_vertices_map) {
      if (with_faces_map) return py::make_tuple(mesh, data.vmap, data.fmap);
      return py::make_tuple(mesh, data.vmap);
    } else {
      if (with_faces_map) return py::make_tuple(mesh, data.fmap);
    }
    return mesh;
  };

  module.def(
      "make_mesh",
      [process_mesh_output](wutils::Coordinates_array<Point>& vertices,
                            wutils::Face_connectivity<Surface_mesh>& faces,
                            bool with_vertices_map,
                            bool with_faces_map) -> py::object {
        wutils::Extension_data<Surface_mesh> data;
        auto mesh = wutils::make_mesh<Surface_mesh>(
            vertices, py::make_tuple(faces), data);
        return process_mesh_output(mesh.release(), data, with_vertices_map,
                                   with_faces_map);
      },
      py::arg("vertices").none(false), py::arg("faces").none(false),
      py::kw_only(), py::arg("with_vertices_map") = false,
      py::arg("with_faces_map") = false);
  module.def(
      "make_mesh",
      [process_mesh_output](wutils::Coordinates_array<Point>& vertices,
                            py::list& faces, bool with_vertices_map,
                            bool with_faces_map) -> py::object {
        wutils::Extension_data<Surface_mesh> data;
        auto mesh = wutils::make_mesh<Surface_mesh>(vertices, faces, data);
        return process_mesh_output(mesh.release(), data, with_vertices_map,
                                   with_faces_map);
      },
      py::arg("vertices").none(false), py::arg("faces").none(false),
      py::kw_only(), py::arg("with_vertices_map") = false,
      py::arg("with_faces_map") = false);

  wutils::wrap_property_map<Vertex_index>(module, pyclass, "vertex");
  wutils::wrap_property_map<Halfedge_index>(module, pyclass, "halfedge");
  wutils::wrap_property_map<Edge_index>(module, pyclass, "edge");
  wutils::wrap_property_map<Face_index>(module, pyclass, "face");

  return pyclass;
}

}  // namespace pyCGAL
