#pragma once

#include <CGAL/Polygon_mesh_processing/corefinement.h>
#include <pyCGAL/typedefs.h>

#include <stdexcept>

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
#ifndef NDEBUG
        if (!CGAL::is_triangle_mesh(tm1) || !CGAL::is_triangle_mesh(tm2))
          throw std::runtime_error("Only triangle meshes can be corefined!");
#endif
        auto np1 = CGAL::Polygon_mesh_processing::parameters::
            throw_on_self_intersection(throw_on_self_intersection);
        auto np2 = CGAL::Polygon_mesh_processing::parameters::
            throw_on_self_intersection(throw_on_self_intersection);
        using Edge_index = typename TriangleMesh::Edge_index;
        if (!edge_is_constrained_map1.is_none()) {
          np1.edge_is_constrained_map(
              *utils::convert_to_property_flag<Edge_index>(
                  edge_is_constrained_map1, tm1));
        }
        if (!edge_is_constrained_map2.is_none()) {
          np2.edge_is_constrained_map(
              *utils::convert_to_property_flag<Edge_index>(
                  edge_is_constrained_map2, tm2));
        }
        CGAL::Polygon_mesh_processing::corefine(tm1, tm2, np1, np2);
      },
      py::arg("tm1").none(false), py::arg("tm2").none(false),
      /** FIXME: new in pybind11 2.6.0: py::kw_only(),*/
      py::arg("edge_is_constrained_map1") = py::none(),
      py::arg("edge_is_constrained_map2") = py::none(),
      py::arg("throw_on_self_intersection") = false);
}
}  // namespace pyCGAL
