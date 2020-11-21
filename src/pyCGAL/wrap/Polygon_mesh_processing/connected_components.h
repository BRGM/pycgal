#pragma once

#include <CGAL/Polygon_mesh_processing/connected_components.h>
#include <pyCGAL/typedefs.h>
#include <pyCGAL/wrap/utils/named_parameters.h>

#include "detail/connected_components.h"
#include "utils/property_helpers.h"

namespace pyCGAL {

namespace detail = pyCGAL::wrap::Polygon_mesh_processing::detail;
namespace utils = pyCGAL::wrap::utils;

template <typename PolygonMesh>
void wrap_element(detail::connected_components<PolygonMesh>,
                  py::module& module) {
  using Edge_index = typename PolygonMesh::Edge_index;
  using Face_index = typename PolygonMesh::Face_index;

  module.def(
      "connected_components",
      [](PolygonMesh& pmesh, py::object fcm,
         py::object edge_is_constrained_map) {
        auto fmap =
            utils::convert_to_property_map<Face_index, int>(fcm, pmesh, 0);
        assert(fmap);

        auto edge_constraints = utils::convert_to_property_flag<Edge_index>(
            edge_is_constrained_map, pmesh);

        namespace pns = CGAL::Polygon_mesh_processing::parameters;

        if (edge_constraints) {
          CGAL::Polygon_mesh_processing::connected_components(
              pmesh, *fmap, pns::edge_is_constrained_map(*edge_constraints));
        } else {
          CGAL::Polygon_mesh_processing::connected_components(pmesh, *fmap);
        }
      },
      py::arg("pmesh").none(false), py::arg("fcm").none(false),
      py::arg("edge_is_constrained_map") = py::none());

  module.def("remove_connected_components",
             [](PolygonMesh& pmesh,
                const std::vector<Face_index>& components_to_remove) {
               CGAL::Polygon_mesh_processing::remove_connected_components(
                   pmesh, CGAL::make_range(begin(components_to_remove),
                                           end(components_to_remove)));
             });

  module.def(
      "remove_connected_components",
      [](PolygonMesh& pmesh, py::iterable components_to_remove) {
        std::vector<Face_index> faces;
        faces.reserve(py::len(components_to_remove));
        for (auto&& x : components_to_remove) {
          faces.emplace_back(x.cast<Face_index>());
        }
        CGAL::Polygon_mesh_processing::remove_connected_components(
            pmesh, CGAL::make_range(begin(faces), end(faces)));
      },
      py::arg("pmesh").none(false),
      py::arg("components_to_remove").none(false));
}

}  // namespace pyCGAL
