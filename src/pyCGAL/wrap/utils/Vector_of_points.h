#pragma once

#include <CGAL/Dimension.h>
#include <pyCGAL/typedefs.h>
#include <pybind11/numpy.h>

#include "CGAL_FT.h"
#include "Vector.h"

namespace pyCGAL {

namespace detail {

template <typename Point>
void extend_vector_of_points_with_array(
    std::vector<Point>& v,
    py::array_t<wrap::utils::CGAL_FT<Point>,
                py::array::c_style | py::array::forcecast>& a) {
  if (a.ndim() != 2) {
    throw std::runtime_error("Inconsistent array dimension.");
  }
  if (a.shape(1) != CGAL::Ambient_dimension<Point>::value) {
    throw std::runtime_error("Inconsistent point dimension.");
  }
  const auto n = a.shape(0);
  v.reserve(v.size() + n);
  for (py::ssize_t k = 0; k != n; ++k) {
    v.emplace_back(*reinterpret_cast<const Point*>(a.data(k, 0)));
  }
}

}  // namespace detail

template <typename Point>
typename WrapTraits<std::vector<Point>>::py_class wrap_class(
    WrapTraits<std::vector<Point>> wrap, py::module& module) {
  using Wrap = WrapTraits<std::vector<Point>>;
  using Vector = typename Wrap::cpp_type;

  typename Wrap::py_class pyclass{module, wrap.name, py::buffer_protocol()};

  wrap_vector_base(pyclass);

  pyclass.def(
      py::init([](py::array_t<wrap::utils::CGAL_FT<Point>,
                              py::array::c_style | py::array::forcecast>& a) {
        auto v = std::make_unique<Vector>();
        detail::extend_vector_of_points_with_array(*v, a);
        return v;
      }));

  pyclass.def_buffer([](Vector& self) {
    using FT = wrap::utils::CGAL_FT<Point>;
    constexpr auto dim = CGAL::Ambient_dimension<Point>::value;
    static_assert(dim * sizeof(FT) == sizeof(Point), "Inconsistent sizes!");
    std::vector<py::ssize_t> shape;
    shape.push_back(self.size());
    shape.push_back(dim);
    std::vector<py::ssize_t> strides;
    strides.push_back(dim * sizeof(FT));
    strides.push_back(sizeof(FT));
    return py::buffer_info(reinterpret_cast<FT*>(self.data()), sizeof(FT),
                           py::format_descriptor<FT>::format(), 2, shape,
                           strides);
  });

  pyclass.def("extend", &detail::extend_vector_of_points_with_array<Point>);

  pyclass.def("__str__", [wrap](const Vector& self) {
    std::stringstream s;
    s << wrap.name << "[";
    auto n = self.size();
    if (n <= 4) {
      for (auto&& P : self) {
        s << "(" << P << ")";
        --n;
        if (n != 0) s << ", ";
      }
    } else {
      s << "(" << self[0] << "), ";
      s << "(" << self[1] << "), ..., ";
      s << "(" << self[n - 2] << "), ";
      s << "(" << self[n - 1] << ")";
    }
    s << "]";
    return s.str();
  });

  return pyclass;
}

}  // namespace pyCGAL
