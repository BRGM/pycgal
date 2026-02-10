#pragma once

#include <pyCGAL/typedefs.h>

#include "extensions/Extended_mesh.h"

namespace pyCGAL {

namespace ext = extensions;

template <typename Mesh>
typename WrapTraits<ext::Surface_soup::Extended_mesh<Mesh>>::py_class
wrap_class(WrapTraits<ext::Surface_soup::Extended_mesh<Mesh>> wrap,
           py::module& module) {
  using EMesh = ext::Surface_soup::Extended_mesh<Mesh>;

  import_dependencies<EMesh>();

  auto pyclass = py::class_<EMesh>(module, wrap.name);

  pyclass.def_property_readonly("mesh", py::overload_cast<>(&EMesh::mesh));
  pyclass.def("number_of_constrained_edges",
              &EMesh::number_of_constrained_edges);

  return pyclass;
}

}  // namespace pyCGAL
