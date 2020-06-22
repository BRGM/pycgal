#pragma once

#include <CGAL/Point_3.h>
#include <CGAL/Vector_3.h>
#include <pyCGAL/Wrapper.h>

#include "utils/point_like.h"

namespace pyCGAL {

template <typename Kernel>
struct Wrapper<CGAL::Point_3<Kernel>> {
  using cgal_type = CGAL::Point_3<Kernel>;
  using pybind_class = py::class_<cgal_type>;
  static pybind_class wrap(py::module& module) {
    return wrap::utils::wrap_point_like<cgal_type, CGAL::Vector_3<Kernel>>(
        module, "Point_3");
  }
};

}  // namespace pyCGAL
