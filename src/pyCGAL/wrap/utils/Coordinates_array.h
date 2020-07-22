#pragma once

#include <pybind11/numpy.h>

#include "CGAL_FT.h"
namespace py = pybind11;

namespace pyCGAL {
namespace wrap {
namespace utils {

template <typename Point>
using Coordinates_array =
    py::array_t<CGAL_FT<Point>, py::array::c_style | py::array::forcecast>;

}  // namespace utils
}  // namespace wrap
}  // namespace pyCGAL
