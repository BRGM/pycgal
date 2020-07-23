#pragma once

#include <CGAL/Point_3.h>
#include <CGAL/Vector_3.h>
#include <pyCGAL/typedefs.h>

#include "utils/point_like.h"

namespace pyCGAL {

template <typename Kernel>
typename WrapTraits<CGAL::Point_3<Kernel>>::py_class wrap_class(
    WrapTraits<CGAL::Point_3<Kernel>>, py::module& module) {
  using Wrap = WrapTraits<CGAL::Point_3<Kernel>>;
  return wrap::utils::wrap_point_like<typename Wrap::cpp_type,
                                      CGAL::Vector_3<Kernel>>(module,
                                                              "Point_3");
}

}  // namespace pyCGAL
