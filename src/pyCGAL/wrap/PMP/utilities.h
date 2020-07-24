#pragma once

#include <CGAL/boost/graph/helpers.h>
#include <pyCGAL/typedefs.h>

namespace pyCGAL {

namespace PMP {

template <typename PolygonMesh>
struct utilities {};

}  // namespace PMP

template <typename PolygonMesh>
void wrap_algorithm(PMP::utilities<PolygonMesh>, py::module& module) {
  module.def("is_triangle_mesh", &CGAL::is_triangle_mesh<PolygonMesh>);
}

}  // namespace pyCGAL
