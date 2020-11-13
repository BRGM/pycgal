#pragma once

#include <pybind11/numpy.h>

namespace py = pybind11;

namespace pyCGAL::wrap::utils {

namespace detail {
// An iterator that returns a copy when dereferenced
template <typename Iterator>
struct Copy_iterator : Iterator {
  using value_type = typename Iterator::value_type;
  // CHECKME: why can't we use:
  // using Iterator::template Iterator;
  // to inherit Iterator constructors?
  Copy_iterator(const Iterator& i) : Iterator(i) {}
  value_type operator*() { return this->Iterator::operator*(); }
};
template <typename Iterator>
auto make_copy_iterator(const Iterator& i) {
  return Copy_iterator<Iterator>(i);
}
}  // namespace detail

template <typename Range>
auto wrap_range(py::module& module, const std::string& name) {
  py::class_<Range> pyclass{module, name.c_str()};
  pyclass.def(
          "__iter__",
          [](const Range& self) {
            // We need to make a copy of dereferenced operator
            // otherwise will be always referencing the same iterator facade
            // which can be a problem in python e.g list(Range)
            // will be a list filled with the latest element of Range
            // CHECKME: is there a huge performance penalty to make copy of indices?!!
            auto b = detail::make_copy_iterator(begin(self));
            auto e = detail::make_copy_iterator(end(self));
            return py::make_iterator(b,e);
          } /*, // We do not need keep_alive here because range does not hold the data... be cautious!
          py::keep_alive<0,1>()*/);
  pyclass.def("__len__", &Range::size);
  pyclass.def_property_readonly("size", &Range::size);
  return pyclass;
}

}  // namespace pyCGAL::wrap::utils
