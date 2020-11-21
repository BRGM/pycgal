#pragma once

#include <CGAL/Polygon_mesh_processing/corefinement.h>
#include <pyCGAL/typedefs.h>
#include <pyCGAL/wrap/utils/named_parameters.h>

#include "detail/corefine.h"
#include "utils/property_helpers.h"

namespace pyCGAL {

namespace detail = pyCGAL::wrap::Polygon_mesh_processing::detail;
namespace utils = pyCGAL::wrap::utils;

template <typename TriangleMesh>
void wrap_element(detail::corefine<TriangleMesh>, py::module& module) {
  module.def(
      "corefine",
      [](TriangleMesh& tm1, TriangleMesh& tm2,
         py::object edge_is_constrained_map1,
         py::object edge_is_constrained_map2,
         const bool throw_on_self_intersection) {
        if (!CGAL::is_triangle_mesh(tm1) || !CGAL::is_triangle_mesh(tm2))
          throw std::runtime_error("Only triangle meshes can be corefined!");

        using Edge_index = typename TriangleMesh::Edge_index;
        using Edge_flag =
            typename TriangleMesh::template Property_map<Edge_index, bool>;

        namespace pns = CGAL::Polygon_mesh_processing::parameters;

        auto edge_constraints1 = utils::convert_to_property_flag<Edge_index>(
            edge_is_constrained_map1, tm1);
        auto edge_constraints_option1 = utils::create_optional_parameter(
            pns::edge_is_constrained_map<Edge_flag>, edge_constraints1);

        auto edge_constraints2 = utils::convert_to_property_flag<Edge_index>(
            edge_is_constrained_map2, tm2);
        auto edge_constraints_option2 = utils::create_optional_parameter(
            pns::edge_is_constrained_map<Edge_flag>, edge_constraints2);

        // WARNING: the handling of BGL Named_parameters needs all this burden
        std::visit(
            [&](auto&& np1, auto&& np2) {
              CGAL::Polygon_mesh_processing::corefine(tm1, tm2, np1, np2);
            },
            utils::concatenate(
                pns::throw_on_self_intersection(throw_on_self_intersection),
                edge_constraints_option1),
            utils::concatenate(
                pns::throw_on_self_intersection(throw_on_self_intersection),
                edge_constraints_option2));
      },
      py::arg("tm1").none(false), py::arg("tm2").none(false), py::kw_only(),
      py::arg("edge_is_constrained_map1") = py::none(),
      py::arg("edge_is_constrained_map2") = py::none(),
      py::arg("throw_on_self_intersection") = false);
}
}  // namespace pyCGAL
