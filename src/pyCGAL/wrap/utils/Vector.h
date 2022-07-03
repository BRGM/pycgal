#pragma once

#include <pybind11/pybind11.h>

#include <vector>

namespace py = pybind11;

namespace pyCGAL {

namespace detail {

template <typename T>
void extend_vector(std::vector<T>& v, const py::list& l) {
  v.reserve(v.size() + py::len(l));
  for (auto&& x : l) {
    v.emplace_back(x.cast<T>());
  }
};

}  // namespace detail

template <typename T>
auto wrap_vector_base(py::class_<std::vector<T>>& pyclass)
    -> py::class_<std::vector<T>>& {
  using Vector = std::vector<T>;

  pyclass.def(py::init<>());
  pyclass.def(py::init<const Vector&>());
  pyclass.def(py::init([](py::list& l) {
    auto v = std::make_unique<Vector>();
    detail::extend_vector(*v, l);
    return v;
  }));

  pyclass.def("__len__", &Vector::size);
  pyclass.def("size", &Vector::size);
  pyclass.def("empty", &Vector::empty);
  pyclass.def_property_readonly("is_empty", &Vector::empty);
  pyclass.def("clear", &Vector::clear);
  pyclass.def("capacity", &Vector::capacity);
  pyclass.def("reserve", &Vector::reserve);
  pyclass.def("shrink_to_fit", &Vector::shrink_to_fit);
  pyclass.def("push_back", py::overload_cast<const T&>(&Vector::push_back));
  pyclass.def("extend", &detail::extend_vector<T>);

  pyclass.def("pop_back", &Vector::pop_back);
  pyclass.def("erase",
              [](Vector& self, std::size_t k) { self.erase(begin(self) + k); });
  pyclass.def("insert", [](Vector& self, std::size_t k, const T& t) {
    self.insert(begin(self) + k, t);
  });
  pyclass.def(
      "__iter__",
      [](Vector& self) { return py::make_iterator(begin(self), end(self)); },
      py::keep_alive<0, 1>());
  pyclass.def("__getitem__",
              py::overload_cast<std::size_t>(&Vector::operator[]));
  pyclass.def("__setitem__",
              [](Vector& self, std::size_t k, const T& t) { self[k] = t; });

  return pyclass;
}

}  // namespace pyCGAL
