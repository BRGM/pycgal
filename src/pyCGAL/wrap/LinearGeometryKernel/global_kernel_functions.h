#pragma once

#include <CGAL/Kernel/global_functions.h>
#include <CGAL/Kernel_traits.h>
#include <CGAL/squared_distance_2.h>
#include <CGAL/squared_distance_3.h>
#include <pyCGAL/typedefs.h>

#include "detail/global_kernel_functions.h"

namespace pyCGAL {

namespace detail = pyCGAL::wrap::LinearGeometryKernel::detail;

namespace {
template <typename T1, typename T2 = T1>
void add_squared_distance(py::module& module) {
  using Kernel = typename CGAL::Kernel_traits<T1>::Kernel;
  static_assert(
      std::is_same_v<Kernel, typename CGAL::Kernel_traits<T2>::Kernel>);
  module.def("squared_distance", py::overload_cast<const T1&, const T2&>(
                                     &CGAL::squared_distance<Kernel>));
  if constexpr (!std::is_same_v<T1, T2>) {
    module.def("squared_distance", py::overload_cast<const T2&, const T1&>(
                                       &CGAL::squared_distance<Kernel>));
  }
}
}  // namespace

template <typename Kernel>
void wrap_element(detail::global_kernel_functions<Kernel>, py::module& module) {
  using FT = typename Kernel::FT;
  using Point_2 = CGAL::Point_2<Kernel>;
  using Segment_2 = CGAL::Segment_2<Kernel>;
  using Point_3 = CGAL::Point_3<Kernel>;
  using Vector_3 = CGAL::Vector_3<Kernel>;
  using Segment_3 = CGAL::Segment_3<Kernel>;
  using Plane_3 = CGAL::Plane_3<Kernel>;

  module.def("approximate_angle",
             py::overload_cast<const Point_3&, const Point_3&, const Point_3&>(
                 &CGAL::approximate_angle<Kernel>));
  module.def("approximate_angle",
             py::overload_cast<const Vector_3&, const Vector_3&>(
                 &CGAL::approximate_angle<Kernel>));
  module.def("cross_product",
             py::overload_cast<const Vector_3&, const Vector_3&>(
                 &CGAL::cross_product<Kernel>));
  add_squared_distance<Point_2>(module);
  add_squared_distance<Segment_2>(module);
  add_squared_distance<Point_2, Segment_2>(module);
  add_squared_distance<Point_3>(module);
  add_squared_distance<Segment_3>(module);
  add_squared_distance<Plane_3>(module);
  add_squared_distance<Point_3, Segment_3>(module);
  add_squared_distance<Point_3, Plane_3>(module);
  module.def("midpoint", py::overload_cast<const Point_2&, const Point_2&>(
                             &CGAL::midpoint<Kernel>));
  module.def("midpoint", py::overload_cast<const Point_3&, const Point_3&>(
                             &CGAL::midpoint<Kernel>));
  module.def("midpoint",
             py::overload_cast<const Segment_2&>(&CGAL::midpoint<Kernel>));
  module.def("midpoint",
             py::overload_cast<const Segment_3&>(&CGAL::midpoint<Kernel>));
  module.def("barycenter",
             py::overload_cast<const Point_2&, const FT&, const Point_2&>(
                 &CGAL::barycenter<Kernel>));
  module.def(
      "barycenter",
      py::overload_cast<const Point_2&, const FT&, const Point_2&, const FT&>(
          &CGAL::barycenter<Kernel>));
  module.def(
      "barycenter",
      py::overload_cast<const Point_2&, const FT&, const Point_2&, const FT&,
                        const Point_2&>(&CGAL::barycenter<Kernel>));
  module.def(
      "barycenter",
      py::overload_cast<const Point_2&, const FT&, const Point_2&, const FT&,
                        const Point_2&, const FT&>(&CGAL::barycenter<Kernel>));
  module.def("barycenter",
             py::overload_cast<const Point_3&, const FT&, const Point_3&>(
                 &CGAL::barycenter<Kernel>));
  module.def(
      "barycenter",
      py::overload_cast<const Point_3&, const FT&, const Point_3&, const FT&>(
          &CGAL::barycenter<Kernel>));
  module.def(
      "barycenter",
      py::overload_cast<const Point_3&, const FT&, const Point_3&, const FT&,
                        const Point_3&>(&CGAL::barycenter<Kernel>));
  module.def(
      "barycenter",
      py::overload_cast<const Point_3&, const FT&, const Point_3&, const FT&,
                        const Point_3&, const FT&>(&CGAL::barycenter<Kernel>));
  module.def("collinear",
             py::overload_cast<const Point_2&, const Point_2&, const Point_2&>(
                 &CGAL::collinear<Kernel>));
  module.def("collinear",
             py::overload_cast<const Point_3&, const Point_3&, const Point_3&>(
                 &CGAL::collinear<Kernel>));
}

}  // namespace pyCGAL
