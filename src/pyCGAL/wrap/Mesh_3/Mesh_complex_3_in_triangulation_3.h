#pragma once

#include <CGAL/Mesh_complex_3_in_triangulation_3.h>
#include <pyCGAL/typedefs.h>

#include "utils/c3t3_connected_components.h"
#include "utils/c3t3_to_arrays.h"

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

  return pyclass;
}

}  // namespace pyCGAL
