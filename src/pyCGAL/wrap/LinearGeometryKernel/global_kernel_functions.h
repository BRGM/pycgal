#pragma once

#include <CGAL/Kernel/global_functions.h>
#include <pyCGAL/typedefs.h>

#include "detail/global_kernel_functions.h"

namespace pyCGAL {

namespace detail = pyCGAL::wrap::LinearGeometryKernel::detail;

template <typename Kernel>
void wrap_element(detail::global_kernel_functions<Kernel>, py::module& module) {
  using Point_3 = CGAL::Point_3<Kernel>;
  using Vector_3 = CGAL::Vector_3<Kernel>;

  module.def("approximate_angle",
             py::overload_cast<const Point_3&, const Point_3&, const Point_3&>(
                 &CGAL::approximate_angle<Kernel>));
  module.def("approximate_angle",
             py::overload_cast<const Vector_3&, const Vector_3&>(
                 &CGAL::approximate_angle<Kernel>));
  module.def("cross_product",
             py::overload_cast<const Vector_3&, const Vector_3&>(
                 &CGAL::cross_product<Kernel>));
}

}  // namespace pyCGAL
