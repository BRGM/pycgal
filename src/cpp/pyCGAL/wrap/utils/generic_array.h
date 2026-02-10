#pragma once

#include <CGAL/Dimension.h>
#include <pybind11/numpy.h>

#include <array>

#include "CGAL_FT.h"

namespace py = pybind11;

namespace pyCGAL {
namespace wrap {
namespace utils {

template <typename Row, typename Component, int dim>
struct Generic_array {
  typedef Row Row_type;
  typedef Component Component_type;

 private:
  Row_type* begin_ptr;
  Row_type* end_ptr;
  std::size_t n;

 public:
  template <int ExtraFlags>
  Generic_array(py::array_t<Component_type, ExtraFlags>& array) {
    static_assert(py::array::c_style & ExtraFlags, "Only C style arrays!");
    static_assert(sizeof(Row_type) == dim * sizeof(Component_type),
                  "Inconsitent component size!");
    if (array.size() % dim != 0)
      throw std::runtime_error("Inconsistent array size!");
    n = static_cast<std::size_t>(array.size() / static_cast<std::size_t>(dim));
    begin_ptr = reinterpret_cast<Row_type*>(array.request().ptr);
    end_ptr = begin_ptr + n;
  }
  auto begin() const { return begin_ptr; }
  auto end() const { return end_ptr; }
  auto size() const { return n; }
};

template <typename Point, int ExtraFlags>
auto make_point_array(py::array_t<CGAL_FT<Point>, ExtraFlags>& array) {
  return Generic_array<Point, CGAL_FT<Point>,
                       CGAL::Ambient_dimension<Point>::value>{array};
}

template <typename Tcast, int dim, typename T, int ExtraFlags>
auto make_array_of_array(py::array_t<T, ExtraFlags>& array) {
  return Generic_array<std::array<Tcast, dim>, T, dim>{array};
}

template <typename Input_iterator>
auto points_as_pyarray(Input_iterator first, Input_iterator last) {
  typedef decltype(*first) Point;
  typedef CGAL_FT<Point> FT;
  const auto n = std::distance(first, last);
  constexpr std::size_t dim =
      static_cast<std::size_t>(CGAL::Ambient_dimension<Point>::value);
  return py::array_t<FT, py::array::c_style>{
      {n, dim}, {dim * sizeof(FT), sizeof(FT)}, &(*first)};
}

template <typename Range>
py::buffer_info points_as_buffer(Range& range) {
  auto b = begin(range);
  typedef typename decltype(b)::value_type Point;
  constexpr std::size_t dim = CGAL::Ambient_dimension<Point>::value;
  typedef CGAL_FT<Point> FT;
  static_assert(dim * sizeof(FT) == sizeof(Point), "Inconsistent sizes!");
  const std::size_t n = std::distance(b, end(range));
  return py::buffer_info(reinterpret_cast<FT*>(&(*b)), {n, dim},
                         {sizeof(Point), sizeof(FT)});
}

}  // namespace utils
}  // namespace wrap
}  // namespace pyCGAL
