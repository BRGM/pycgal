#pragma once

// CHECKME: Weirdly enough the Constrained_Delaunay_triangulation_2 is mandatory
// (is this a CGAL "bug" ?)
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/lloyd_optimize_mesh_2.h>
#include <pyCGAL/typedefs.h>

#include "detail/lloyd_optimize_mesh_2.h"

namespace pyCGAL {

namespace detail = pyCGAL::wrap::Mesh_2::detail;

template <typename Constrained_Delaunay_triangulation>
void wrap_element(
    detail::lloyd_optimize_mesh_2<Constrained_Delaunay_triangulation>,
    py::module& module) {
  module.def(
      "lloyd_optimize_mesh_2",
      [](Constrained_Delaunay_triangulation& cdt, const double& time_limit,
         const int& max_iteration_number, const double& convergence,
         const double& freeze_bound) {
        return CGAL::lloyd_optimize_mesh_2(
            cdt, CGAL::parameters::time_limit = time_limit,
            CGAL::parameters::max_iteration_number = max_iteration_number,
            CGAL::parameters::convergence = convergence,
            CGAL::parameters::freeze_bound = freeze_bound);
      },
      py::arg("cdt"), py::arg("time_limit") = 0,
      py::arg("max_iteration_number") = 0, py::arg("convergence") = 0.001,
      py::arg("freeze_bound") = 0.001);
}

}  // namespace pyCGAL
