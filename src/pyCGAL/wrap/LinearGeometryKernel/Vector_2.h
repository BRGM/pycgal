#pragma once

#include <CGAL/Point_2.h>
#include <CGAL/Vector_2.h>
#include <pyCGAL/typedefs.h>

#include "utils/point_like.h"
#include "utils/vector_operators.h"

namespace pyCGAL {

template <typename Kernel>
typename WrapTraits<CGAL::Vector_2<Kernel>>::py_class wrap_class(
    WrapTraits<CGAL::Vector_2<Kernel>>, py::module& module) {
  using Wrap = WrapTraits<CGAL::Vector_2<Kernel>>;
  typename Wrap::py_class pyclass =
      wrap::utils::wrap_point_like<typename Wrap::cpp_type,
                                   CGAL::Vector_2<Kernel>>(module, "Vector_2");
  wrap::utils::wrap_vector_operators(pyclass);
  return pyclass;
}

}  // namespace pyCGAL
