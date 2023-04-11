#pragma once

#include <pybind11/numpy.h>

#include <algorithm>
#include <vector>

namespace py = pybind11;

namespace pyCGAL::wrap::utils {

template <typename Point>
auto polyline_to_array(const std::vector<Point>& polyline) {
  constexpr py::ssize_t dim = 3;
  static_assert(sizeof(Point) == dim * sizeof(double));
  auto result = py::array_t<double, py::array::c_style>{
      {(py::ssize_t)polyline.size(), dim}};
  copy(begin(polyline), end(polyline),
       reinterpret_cast<Point*>(result.mutable_data(0, 0)));
  return result;
}

template <typename Polylines>
auto polylines_to_list(const Polylines& polylines) {
  py::list result;
  for (auto&& polyline : polylines) {
    result.append(polyline_to_array(polyline));
  }
  return result;
}

}  // namespace pyCGAL::wrap::utils
