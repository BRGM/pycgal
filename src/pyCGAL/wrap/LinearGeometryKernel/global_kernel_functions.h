#pragma once

#include <CGAL/Kernel/global_functions.h>
#include <CGAL/squared_distance_2.h>
#include <CGAL/squared_distance_3.h>
#include <pyCGAL/typedefs.h>

#include "detail/global_kernel_functions.h"

namespace pyCGAL {

namespace detail = pyCGAL::wrap::LinearGeometryKernel::detail;

template <typename Kernel>
void wrap_element(detail::global_kernel_functions<Kernel>, py::module& module) {
  using Point_2 = CGAL::Point_2<Kernel>;
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
  module.def("squared_distance",
             py::overload_cast<const Point_2&, const Point_2&>(
                 &CGAL::squared_distance<Kernel>));
  module.def("squared_distance",
             py::overload_cast<const Point_3&, const Point_3&>(
                 &CGAL::squared_distance<Kernel>));
}

}  // namespace pyCGAL
