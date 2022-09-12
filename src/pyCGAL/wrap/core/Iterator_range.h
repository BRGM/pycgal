#pragma once

#include <CGAL/Iterator_range.h>
#include <pyCGAL/typedefs.h>

namespace pyCGAL {

namespace core {
namespace detail {

// py::make_iterator fails when building list from iterator
// __next__ must return a value (py::object) not a reference to a py::object
template <typename IRange>
struct Iterator_range_iterator {
  using iterator = typename IRange::iterator;
  using value_type = typename iterator::value_type;
  IRange range;
  iterator state;
  Iterator_range_iterator(IRange r) : range{r}, state{begin(r)} {}
  auto __iter__() {
    // reset to a new iterator
    return Iterator_range_iterator{range};
  }
  value_type __next__() {
    if (state == end(range)) throw py::stop_iteration{};
    return *(state++);
  }
};

}  // namespace detail
}  // namespace core

template <typename I>
typename WrapTraits<CGAL::Iterator_range<I>>::py_class wrap_class(
    WrapTraits<CGAL::Iterator_range<I>> wrap, py::module& module) {
  using Wrap = WrapTraits<CGAL::Iterator_range<I>>;
  using Iterator_range = typename Wrap::cpp_type;
  using wrapped_iterator =
      core::detail::Iterator_range_iterator<Iterator_range>;

  py::class_<wrapped_iterator>(module,
                               (wrap.name + std::string{"_iterator"}).c_str())
      .def("__iter__", &wrapped_iterator::__iter__, py::keep_alive<0, 1>())
      .def("__next__", &wrapped_iterator::__next__);

  auto pyclass = py::class_<Iterator_range>(module, wrap.name);
  pyclass.def(
      "__iter__", [](Iterator_range& self) { return wrapped_iterator{self}; },
      py::keep_alive<0, 1>());
  pyclass.def("__len__", [](const Iterator_range& self) {
    return std::distance(begin(self), end(self));
  });

  return pyclass;
}

}  // namespace pyCGAL
