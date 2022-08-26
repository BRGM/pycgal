#pragma once

#include <pybind11/numpy.h>

#include <vector>

namespace py = pybind11;

namespace pyCGAL::wrap::utils {

namespace {
template <typename T>
struct Container_proxy {
  // we keep a (shallow) copy of the source object to prevent garbage collection
  py::object source;
  using Container = std::vector<T>;
  Container tmp_copy;
  const Container* container = nullptr;
  Container_proxy(py::object obj) : source{obj} {
    assert(container == nullptr);
    assert(tmp_copy.empty());
    if (source.is_none()) {
      container = &tmp_copy;
      return;
    }
    try {
      container = source.cast<const Container*>();
    } catch (py::cast_error) {
      container = nullptr;
    }
    if (!container) {
      container = &tmp_copy;
      try {
        for (auto&& x : source) {
          tmp_copy.emplace_back(x.cast<T>());
        }
      } catch (py::cast_error) {
        container = nullptr;
      }
    }
    if (!container) {
      container = &tmp_copy;
      try {
        tmp_copy.emplace_back(source.cast<T>());
      } catch (py::cast_error) {
        throw std::runtime_error("Could not cast underlying object.");
      }
    }
    assert(container);
  }
  auto as_range() const {
    return CGAL::make_range(begin(*container), end(*container));
  }
};

}  // namespace

template <typename T>
auto container_proxy(py::object& source) {
  return Container_proxy<T>{source};
}

}  // namespace pyCGAL::wrap::utils
