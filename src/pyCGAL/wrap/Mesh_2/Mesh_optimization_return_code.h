#pragma once

#include <CGAL/Mesh_optimization_return_code.h>
#include <pyCGAL/typedefs.h>

namespace pyCGAL {

void wrap_element(CGALEnum<CGAL::Mesh_optimization_return_code>,
                  py::module& module) {
  py::enum_<CGAL::Mesh_optimization_return_code>(
      module, "Mesh_optimization_return_code")
      .value("BOUND_REACHED", CGAL::BOUND_REACHED)
      .value("TIME_LIMIT_REACHED", CGAL::TIME_LIMIT_REACHED)
      .value("CANT_IMPROVE_ANYMORE", CGAL::CANT_IMPROVE_ANYMORE)
      .value("CONVERGENCE_REACHED", CGAL::CONVERGENCE_REACHED)
      .value("MAX_ITERATION_NUMBER_REACHED", CGAL::MAX_ITERATION_NUMBER_REACHED)
      .value("ALL_VERTICES_FROZEN", CGAL::ALL_VERTICES_FROZEN);
}

}  // namespace pyCGAL
