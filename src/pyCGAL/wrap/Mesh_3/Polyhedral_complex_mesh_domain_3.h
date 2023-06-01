#pragma once

#include <CGAL/Polygon_mesh_processing/connected_components.h>
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
      py::init([](const Surface_mesh& outer_shell, py::list inner_surfaces,
                  py::object split_along_constraints) {
        using Edge_index = typename Surface_mesh::Edge_index;
        using Face_index = typename Surface_mesh::Face_index;
        std::vector<Polyhedron> polyhedrons;
        std::vector<std::pair<Subdomain_index, Subdomain_index>> indices;
        if (split_along_constraints.is_none()) {
          polyhedrons.emplace_back();
          CGAL::copy_face_graph(outer_shell, polyhedrons.back());
          indices.emplace_back(1, 0);
          for (auto&& S : inner_surfaces) {
            polyhedrons.emplace_back();
            CGAL::copy_face_graph(S.cast<Surface_mesh&>(), polyhedrons.back());
            indices.emplace_back(1, 1);
          }
        } else {
          std::string mapname;
          try {
            mapname = py::cast<std::string>(split_along_constraints);
          } catch (py::cast_error e) {
            throw std::runtime_error("Could not convert edge map name!");
          }
          auto [edge_is_constrained, found] =
              outer_shell.template property_map<Edge_index, bool>(mapname);
          // FIXME: the constraints map could be optional
          if (!found)
            throw std::runtime_error("Could not convert edge map name!");
          std::vector<Surface_mesh> components;
          CGAL::Polygon_mesh_processing::split_connected_components(
              outer_shell, components,
              CGAL::parameters::edge_is_constrained_map(edge_is_constrained));
          std::size_t n = 0;
          for (auto&& e : outer_shell.edges()) {
            if (edge_is_constrained[e]) ++n;
          }
          std::cerr << "Found " << n << " constrained edges and "
                    << components.size()
                    << " connected components on outter shell." << std::endl;
          for (auto&& S : components) {
            polyhedrons.emplace_back();
            CGAL::copy_face_graph(S, polyhedrons.back());
            indices.emplace_back(1, 0);
          }
          for (auto&& S : inner_surfaces) {
            Surface_mesh& mesh = S.cast<Surface_mesh&>();
            std::tie(edge_is_constrained, found) =
                mesh.template property_map<Edge_index, bool>(mapname);
            // FIXME: the constraints map could be optional
            if (!found)
              throw std::runtime_error("Could not convert edge map name!");
            components.clear();
            CGAL::Polygon_mesh_processing::split_connected_components(
                mesh, components,
                CGAL::parameters::edge_is_constrained_map(edge_is_constrained));
            n = 0;
            for (auto&& e : mesh.edges()) {
              if (edge_is_constrained[e]) ++n;
            }
            std::cerr << "Found " << n << " constrained edges and "
                      << components.size() << " connected components on mesh."
                      << std::endl;
            for (auto&& comp : components) {
              polyhedrons.emplace_back();
              CGAL::copy_face_graph(comp, polyhedrons.back());
              indices.emplace_back(1, 1);
            }
          }
        }
        return std::make_unique<Domain>(polyhedrons.begin(), polyhedrons.end(),
                                        indices.begin(), indices.end());
      }),
      py::arg("outer_shell").none(false),
      py::arg("inner_surfaces") = py::list{},
      py::arg("split_along_constraints") = py::none());

  Mesh_3::utils::wrap_common_domain_api<Domain>(pyclass);

  return pyclass;
}

}  // namespace pyCGAL
