#pragma once

#include <CGAL/Polyhedral_mesh_domain_with_features_3.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/boost/graph/copy_face_graph.h>
#include <pyCGAL/typedefs.h>
#include <pyCGAL/wrap/utils/CGAL_FT.h>

namespace pyCGAL {

template <typename Domain>
typename WrapTraits<Domain>::py_class wrap_class(WrapTraits<Domain> wrap,
                                                 py::module& module) {
  using Polyhedron = typename Domain::Polyhedron;
  using Point = typename Domain::Point_3;
  using Surface_mesh = CGAL::Surface_mesh<Point>;

  import_dependencies<Domain>();

  auto pyclass = py::class_<Domain>(module, wrap.name);
  pyclass.def(py::init<const Polyhedron&>());
  pyclass.def(py::init<const Polyhedron&, const Polyhedron&>());

  pyclass.def(py::init([](const Surface_mesh& sm) {
    Polyhedron polyhedron;
    CGAL::copy_face_graph(sm, polyhedron);
    return std::make_unique<Domain>(polyhedron);
  }));
  pyclass.def(
      py::init([](const Surface_mesh& sm, const Surface_mesh& bounding_sm) {
        Polyhedron polyhedron, bounding_polyhedron;
        CGAL::copy_face_graph(sm, polyhedron);
        CGAL::copy_face_graph(bounding_sm, bounding_polyhedron);
        return std::make_unique<Domain>(polyhedron, bounding_polyhedron);
      }));
  pyclass.def(py::init(
      [](const Surface_mesh& sm, const Polyhedron& bounding_polyhedron) {
        Polyhedron polyhedron;
        CGAL::copy_face_graph(sm, polyhedron);
        return std::make_unique<Domain>(polyhedron, bounding_polyhedron);
      }));
  pyclass.def(py::init(
      [](const Polyhedron& polyhedron, const Surface_mesh& bounding_sm) {
        Polyhedron bounding_polyhedron;
        CGAL::copy_face_graph(bounding_sm, bounding_polyhedron);
        return std::make_unique<Domain>(polyhedron, bounding_polyhedron);
      }));

  pyclass.def("detect_borders", [](Domain& self) { self.detect_borders(); });
  pyclass.def(
      "detect_features",
      [](Domain& self, const wrap::utils::CGAL_FT<Point> angle) {
        self.detect_features(angle);
      },
      py::arg("angle") = 60);

  return pyclass;
}

}  // namespace pyCGAL
