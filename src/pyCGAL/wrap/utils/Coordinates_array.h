#pragma once

#include <CGAL/Dimension.h>
#include <pybind11/numpy.h>

#include "CGAL_FT.h"
namespace py = pybind11;

namespace pyCGAL {
namespace wrap {
namespace utils {

template <typename Point,
          int ExtraFlags = py::array::c_style | py::array::forcecast>
using Coordinates_array = py::array_t<CGAL_FT<Point>, ExtraFlags>;

template <typename Point>
using Output_coordinate_array = Coordinates_array<Point, py::array::c_style>;

template <typename Point>
Output_coordinate_array<Point> empty_coordinates_array(const std::size_t n) {
  return Output_coordinate_array<Point>{
      {n, static_cast<std::size_t>(CGAL::Ambient_dimension<Point>::value)}};
}

}  // namespace utils
}  // namespace wrap
}  // namespace pyCGAL
