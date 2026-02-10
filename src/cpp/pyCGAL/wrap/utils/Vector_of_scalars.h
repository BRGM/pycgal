#pragma once

#include <pyCGAL/typedefs.h>
#include <pybind11/numpy.h>

#include "Vector.h"
#include "vector_to_ostream.h"

namespace pyCGAL {

namespace detail {

template <typename T>
void extend_vector_of_scalars_with_array(
    std::vector<T>& v,
    py::array_t<T, py::array::c_style | py::array::forcecast>& a) {
  if (a.ndim() != 1) {
    throw std::runtime_error("Inconsistent array dimension.");
  }
  const auto n = a.size();
  v.reserve(v.size() + n);
  for (py::ssize_t k = 0; k != n; ++k) {
    v.emplace_back(*reinterpret_cast<const T*>(a.data(k)));
  }
}

}  // namespace detail

template <typename T>
typename WrapTraits<std::vector<T>>::py_class wrap_class(
    WrapTraits<std::vector<T>> wrap, py::module& module) {
  using Wrap = WrapTraits<std::vector<T>>;
  using Vector = typename Wrap::cpp_type;

  typename Wrap::py_class pyclass{module, wrap.name, py::buffer_protocol()};

  wrap_vector_base(pyclass);

  pyclass.def(py::init(
      [](py::array_t<T, py::array::c_style | py::array::forcecast>& a) {
        auto v = std::make_unique<Vector>();
        detail::extend_vector_of_scalars_with_array(*v, a);
        return v;
      }));

  pyclass.def_buffer(
      [](Vector& self) { return py::buffer_info(self.data(), self.size()); });

  pyclass.def("extend", &detail::extend_vector_of_scalars_with_array<T>);

  pyclass.def("__str__", [wrap](const Vector& self) {
    std::stringstream s;
    s << wrap.name;
    wrap::utils::vector_to_ostream(self, s, "", ", ", "");
    return s.str();
  });

  return pyclass;
}

}  // namespace pyCGAL
