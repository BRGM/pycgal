#pragma once

#include <CGAL/Mesh_criteria_3.h>
#include <CGAL/make_mesh_3.h>
#include <pyCGAL/typedefs.h>

#include "detail/make_mesh_3.h"

namespace pyCGAL {

namespace detail = pyCGAL::wrap::Mesh_3::detail;

template <typename Domain, typename C3t3>
void wrap_element(detail::make_mesh_3<Domain, C3t3>, py::module& module) {
  using Triangulation = typename C3t3::Triangulation;
  using Mesh_criteria = CGAL::Mesh_criteria_3<Triangulation>;

  module.def("make_mesh_3",
             [](const Domain& domain, const Mesh_criteria& criteria) {
               return CGAL::make_mesh_3<C3t3>(domain, criteria,
                                              CGAL::parameters::no_perturb(),
                                              CGAL::parameters::no_exude());
             });
}

}  // namespace pyCGAL
