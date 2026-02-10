#pragma once

#include <CGAL/Mesh_criteria_3.h>
#include <CGAL/make_mesh_3.h>
#include <pyCGAL/typedefs.h>

#include "detail/make_mesh_3.h"

namespace pyCGAL {

namespace detail {

template <typename C3t3, typename Domain>
void wrap_make_mesh_api(py::module& module) {
  using Triangulation = typename C3t3::Triangulation;
  using Mesh_criteria = CGAL::Mesh_criteria_3<Triangulation>;

  module.def("make_mesh_3",
             [](const Domain& domain, const Mesh_criteria& criteria) {
               return CGAL::make_mesh_3<C3t3>(domain, criteria,
                                              CGAL::parameters::no_perturb(),
                                              CGAL::parameters::no_exude());
             });
}

template <typename C3t3, typename Domain, typename... Domains>
void wrap_make_mesh_apis(py::module& module) {
  wrap_make_mesh_api<C3t3, Domain>(module);
  if constexpr (sizeof...(Domains) > 0)
    wrap_make_mesh_apis<C3t3, Domains...>(module);
}

}  // namespace detail

namespace wd = pyCGAL::wrap::Mesh_3::detail;

template <typename C3t3, typename... Domains>
void wrap_element(wd::make_mesh_3<C3t3, Domains...>, py::module& module) {
  detail::wrap_make_mesh_apis<C3t3, Domains...>(module);
}

}  // namespace pyCGAL
