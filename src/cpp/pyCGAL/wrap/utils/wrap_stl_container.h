#pragma once

#include <pybind11/numpy.h>

#include <algorithm>
#include <vector>

namespace py = pybind11;

namespace pyCGAL::wrap::utils {

template <typename value_type, typename cast_type = value_type>
auto wrap_stl_vector(py::module& module, const std::string& name,
                     const bool with_buffer_interface,
                     const bool with_numpy_array_interface) {
  using Vector = std::vector<value_type>;

  auto buffer_info = [](Vector& v) -> py::buffer_info {
    return py::buffer_info(
        reinterpret_cast<std::add_pointer_t<value_type>>(v.data()), v.size());
  };

  auto extend_with_list = [](Vector& v, py::iterable l) -> void {
    v.reserve(v.size() + py::len(l));
    for (auto&& x : l) {
      if constexpr (std::is_same_v<value_type, cast_type>) {
        v.emplace_back(x.cast<value_type>());
      } else {
        try {
          v.emplace_back(x.cast<value_type>());
        } catch (const py::cast_error&) {
          v.emplace_back(x.cast<cast_type>());
        }
      }
    }
  };

  auto extend_with_buffer = [](Vector& v, py::buffer b) -> void {
    py::buffer_info info = b.request();
    if (info.itemsize != sizeof(value_type)) {
      throw std::runtime_error(
          "Buffer items have inconsistent size in memory.");
    }
    if (info.ndim != 1) {
      throw std::runtime_error(
          "Only dim 1 buffers can be used to extend vectors.");
    }
    if (info.strides[0] != info.itemsize) {
      throw std::runtime_error(
          "Only contiguous buffers can be used to extend vectors.");
    }
    auto p = reinterpret_cast<std::add_pointer_t<cast_type>>(info.ptr);
    if (!p) {
      throw std::runtime_error("Null buffer pointer!");
    }
    v.reserve(v.size() + info.shape[0]);
    std::transform(p, p + info.shape[0], back_inserter(v),
                   [](const cast_type& x) { return value_type{x}; });
  };

  auto create_from_iterable = [extend_with_list](py::iterable l) {
    auto v = std::make_unique<Vector>();
    assert(v);
    extend_with_list(*v, l);
    return v;
  };

  auto create_from_array =
      [extend_with_buffer](
          py::array_t<cast_type, py::array::c_style | py::array::forcecast> a) {
        auto v = std::make_unique<Vector>();
        assert(v);
        extend_with_buffer(*v, a);
        return v;
      };

  py::class_<Vector> pyclass =
      with_buffer_interface
          ? py::class_<Vector>{module, name.c_str(), py::buffer_protocol()}
          : py::class_<Vector>{module, name.c_str()};
  if (with_buffer_interface) pyclass.def_buffer(buffer_info);

  pyclass.def(py::init<>());
  pyclass.def(py::init([](py::iterator it) {
    auto self = std::make_unique<Vector>();
    while (it != py::iterator::sentinel()) {
      self->push_back(it->cast<value_type>());
      ++it;
    }
    return self;
  }));
  if constexpr (std::is_same_v<value_type, cast_type>) {
    pyclass.def(py::init(create_from_array));
    pyclass.def(py::init(create_from_iterable));
  } else {
    pyclass.def_static("create", create_from_array);
    pyclass.def_static("create", create_from_iterable);
  }
  if (with_numpy_array_interface) {
    pyclass.def(
        "as_array",
        [buffer_info](Vector& self) {
          return py::array_t<value_type, py::array::c_style>{buffer_info(self)};
        },
        py::keep_alive<0, 1>());
  }
  pyclass.def(
      "__iter__",
      [](Vector& self) { return py::make_iterator(begin(self), end(self)); },
      py::keep_alive<0, 1>());
  pyclass.def("__len__", &Vector::size);
  pyclass.def_property_readonly("size", &Vector::size);
  pyclass.def("__getitem__", [](Vector& self, const std::size_t pos) {
#ifndef NDEBUG
    if (pos >= self.size()) {
      throw std::runtime_error("Bound checking failed...");
    }
#endif
    return self[pos];
  });
  pyclass.def("__setitem__",
              [](Vector& self, const std::size_t pos, const value_type& x) {
#ifndef NDEBUG
                if (pos >= self.size()) {
                  throw std::runtime_error("Bound checking failed...");
                }
#endif
                self[pos] = value_type{x};
              });
  pyclass.def("append",
              [](Vector& self, const value_type x) { self.emplace_back(x); });
  pyclass.def("extend", [](Vector& self, const Vector& other) {
    self.reserve(self.size() + other.size());
    self.insert(end(self), begin(other), end(other));
  });
  pyclass.def("extend", extend_with_list);
  pyclass.def("extend", extend_with_buffer);
  pyclass.def("remove_duplicates", [](Vector& self) {
    sort(begin(self), end(self));
    self.erase(unique(begin(self), end(self)), end(self));
  });
  return pyclass;
}

}  // namespace pyCGAL::wrap::utils
