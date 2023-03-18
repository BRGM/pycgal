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

  pyclass.def("as_arrays", &wrap::utils::c3t3_to_arrays<C3t3>,
              py::arg("return_corner_index"), py::arg("return_curve_index"),
              py::arg("return_facet_index"), py::arg("return_subdomain_index"));

  pyclass.def("connected_components",
              &wrap::utils::connected_components_to_array<C3t3>);

  return pyclass;
}

}  // namespace pyCGAL
