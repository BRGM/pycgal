#pragma once

#include <CGAL/Polyhedron_3.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/boost/graph/copy_face_graph.h>
#include <pyCGAL/typedefs.h>

namespace pyCGAL {

template <typename Polyhedron>
typename WrapTraits<Polyhedron>::py_class wrap_class(
    WrapTraits<Polyhedron> wrap, py::module& module) {
  using Point = typename Polyhedron::Point_3;
  using Surface_mesh = CGAL::Surface_mesh<Point>;

  import_dependencies<Polyhedron>();

  auto pyclass = py::class_<Polyhedron>(module, wrap.name);
  pyclass.def(py::init<>());
  pyclass.def(py::init([](const Surface_mesh& sm) {
    auto p = std::make_unique<Polyhedron>();
    CGAL::copy_face_graph(sm, *p);
    return p;
  }));

  pyclass.def("size_of_vertices", &Polyhedron::size_of_vertices);
  pyclass.def("number_of_vertices", &Polyhedron::size_of_vertices);
  pyclass.def("size_of_facets", &Polyhedron::size_of_facets);
  pyclass.def("number_of_facets", &Polyhedron::size_of_facets);

  return pyclass;
}

}  // namespace pyCGAL
