#pragma once

#include <CGAL/Polyhedral_complex_mesh_domain_3.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/boost/graph/copy_face_graph.h>
#include <pyCGAL/typedefs.h>

#include "utils/common_domain_api.h"

namespace pyCGAL {

template <typename Domain>
typename WrapTraits<Domain>::py_class wrap_class(WrapTraits<Domain> wrap,
                                                 py::module& module) {
  using Polyhedron = typename Domain::Polyhedron;
  using Point = typename Domain::Point_3;
  using Surface_mesh = CGAL::Surface_mesh<Point>;
  using Subdomain_index = typename Domain::Subdomain_index;

  import_dependencies<Domain>();

  auto pyclass = py::class_<Domain>(module, wrap.name);

  pyclass.def(
      py::init([](const Surface_mesh& outer_shell, py::list inner_surfaces) {
        std::vector<Polyhedron> polyhedrons;
        std::vector<std::pair<Subdomain_index, Subdomain_index>> indices;
        polyhedrons.emplace_back();
        CGAL::copy_face_graph(outer_shell, polyhedrons.back());
        indices.emplace_back(1, 0);
        for (auto&& S : inner_surfaces) {
          polyhedrons.emplace_back();
          CGAL::copy_face_graph(S.cast<Surface_mesh&>(), polyhedrons.back());
          indices.emplace_back(1, 1);
        }
        return std::make_unique<Domain>(polyhedrons.begin(), polyhedrons.end(),
                                        indices.begin(), indices.end());
      }),
      py::arg("outer_shell").none(false),
      py::arg("inner_surfaces") = py::list{});

  Mesh_3::utils::wrap_common_domain_api<Domain>(pyclass);

  return pyclass;
}

}  // namespace pyCGAL
