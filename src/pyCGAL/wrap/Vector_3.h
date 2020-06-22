#pragma once

#include <CGAL/Point_3.h>
#include <CGAL/Vector_3.h>
#include <pyCGAL/Wrapper.h>

#include "utils/point_like.h"
#include "utils/vector_operators.h"

namespace pyCGAL {

template <typename Kernel>
struct Wrapper<CGAL::Vector_3<Kernel>> {
  using cgal_type = CGAL::Vector_3<Kernel>;
  using pybind_class = py::class_<cgal_type>;
  static pybind_class wrap(py::module& module) {
    pybind_class pyclass =
        wrap::utils::wrap_point_like<cgal_type, cgal_type>(module, "Vector_3");
    wrap::utils::wrap_vector_operators(pyclass);
    return pyclass;
  }
};

}  // namespace pyCGAL
