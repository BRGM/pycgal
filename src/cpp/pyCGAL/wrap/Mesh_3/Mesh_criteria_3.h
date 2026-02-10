#pragma once

#include <CGAL/Mesh_criteria_3.h>

#include <limits>

namespace pyCGAL {

template <typename Mesh_criteria>
typename WrapTraits<Mesh_criteria>::py_class wrap_class(
    WrapTraits<Mesh_criteria> wrap, py::module& module) {
  using Edge_criteria = typename Mesh_criteria::Edge_criteria;
  using Facet_criteria = typename Mesh_criteria::Facet_criteria;
  using Cell_criteria = typename Mesh_criteria::Cell_criteria;

  auto pyclass = py::class_<Mesh_criteria>(module, wrap.name);
  pyclass.def(
      py::init([](py::object edge_size, py::object facet_angle,
                  py::object facet_size, py::object facet_distance,
                  py::object cell_radius_edge_ratio,
                  py::object cell_radius_edge, py::object cell_size) {
        auto extract = [](py::object param, const double default_value = 0) {
          return param.is_none() ? default_value : param.cast<double>();
        };

        auto edge_criteria = Edge_criteria(
            extract(edge_size, std::numeric_limits<double>::max()));

        auto facet_criteria = Facet_criteria(
            extract(facet_angle), extract(facet_size), extract(facet_distance),
            CGAL::FACET_VERTICES_ON_SURFACE);

        auto cell_criteria = Cell_criteria(
            cell_radius_edge_ratio.is_none() ? extract(cell_radius_edge)
                                             : extract(cell_radius_edge_ratio),
            extract(cell_size));

        return std::make_unique<Mesh_criteria>(edge_criteria, facet_criteria,
                                               cell_criteria);
      }),
      py::kw_only(), py::arg("edge_size") = py::none(),
      py::arg("facet_angle") = py::none(), py::arg("facet_size") = py::none(),
      py::arg("facet_distance") = py::none(),
      py::arg("cell_radius_edge_ratio") = py::none(),
      py::arg("cell_radius_edge") = py::none(),
      py::arg("cell_size") = py::none());

  return pyclass;
}

}  // namespace pyCGAL
