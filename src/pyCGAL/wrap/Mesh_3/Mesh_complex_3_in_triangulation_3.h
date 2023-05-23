#pragma once

#include <CGAL/Mesh_complex_3_in_triangulation_3.h>
#include <pyCGAL/typedefs.h>

#include "utils/c3t3_collection_utilities.h"
#include "utils/c3t3_connected_components.h"
#include "utils/c3t3_to_arrays.h"
#include "utils/c3t3_vertex_connected_component.h"

namespace pyCGAL {

template <typename C3t3>
typename WrapTraits<C3t3>::py_class wrap_class(WrapTraits<C3t3> wrap,
                                               py::module& module) {
  import_dependencies<C3t3>();

  auto pyclass = py::class_<C3t3>(module, wrap.name);
  pyclass.def(py::init<>());

  pyclass.def("number_of_corners", &C3t3::number_of_corners);
  pyclass.def("number_of_edges", &C3t3::number_of_edges);
  pyclass.def("number_of_facets", &C3t3::number_of_facets);
  pyclass.def("number_of_cells",
              [](const C3t3& self) { return self.number_of_cells(); });

  pyclass.def(
      "as_arrays",
      [](const C3t3& self, const bool rebind, const bool no_patch,
         const bool return_corner_index, const bool return_curve_index,
         const bool return_facet_index, const bool return_subdomain_index) {
        using wrap::utils::c3t3_to_arrays;
        if (rebind) {
          if (no_patch) {
            return c3t3_to_arrays<C3t3, true, true>(
                self, return_corner_index, return_curve_index,
                return_facet_index, return_subdomain_index);
          } else {
            return c3t3_to_arrays<C3t3, true, false>(
                self, return_corner_index, return_curve_index,
                return_facet_index, return_subdomain_index);
          }
        } else {
          if (no_patch) {
            return c3t3_to_arrays<C3t3, false, true>(
                self, return_corner_index, return_curve_index,
                return_facet_index, return_subdomain_index);
          } else {
            return c3t3_to_arrays<C3t3, false, false>(
                self, return_corner_index, return_curve_index,
                return_facet_index, return_subdomain_index);
          }
        }
        assert(false);
        return py::list{};
      },
      py::kw_only(), py::arg("rebind") = false, py::arg("no_patch") = false,
      py::arg("return_corner_index") = false,
      py::arg("return_curve_index") = false,
      py::arg("return_facet_index") = false,
      py::arg("return_subdomain_index") = false);

  pyclass.def("connected_components",
              &wrap::utils::connected_components_to_array<C3t3>);
  pyclass.def("all_corner_indices",
              &wrap::utils::collect_all_corner_indices<C3t3>);
  // pyclass.def("vertices_index",
  //             &wrap::utils::collect_vertices_index<C3t3>);
  pyclass.def("vertices_dimension",
              &wrap::utils::collect_vertices_dimension<C3t3>);
  pyclass.def("tets_with_incident_facets",
              &wrap::utils::collect_tets_with_incident_facets<C3t3>);

  pyclass.def("number_of_connected_components_per_vertex",
              [](const C3t3& self) {
                using Triangulation = typename C3t3::Triangulation;
                const Triangulation& tr = self.triangulation();
                using wrap::utils::number_of_connected_components;
                py::array_t<int, py::array::c_style> nbcomps{
                    static_cast<py::ssize_t>(tr.number_of_vertices())};
                int* p = nbcomps.mutable_data();
                for (auto vit = tr.finite_vertices_begin();
                     vit != tr.finite_vertices_end(); ++vit, ++p) {
                  (*p) = number_of_connected_components(self, vit);
                }
                return nbcomps;
              });

  pyclass.def("pack_cells_along_facets", [](const C3t3& self) {
    using wrap::utils::pack_cells_along_facets;
    using Cell_handle = typename C3t3::Cell_handle;
    auto&& packs = pack_cells_along_facets(self);
    py::array_t<int, py::array::c_style> result{
        static_cast<py::ssize_t>(self.number_of_cells())};
    auto compid = result.mutable_data();
    for (auto cit = self.cells_in_complex_begin();
         cit != self.cells_in_complex_end(); ++cit) {
      auto p = packs.find((Cell_handle)cit);
      if (p == packs.end()) {
        (*compid) = 0;
      } else {
        (*compid) = p->second;
      }
      ++compid;
    }
    return result;
  });

  return pyclass;
}

}  // namespace pyCGAL
