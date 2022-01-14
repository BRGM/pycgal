#pragma once

#include <CGAL/Iterator_range.h>
#include <pyCGAL/typedefs.h>

namespace pyCGAL {

template <typename I>
typename WrapTraits<CGAL::Iterator_range<I>>::py_class wrap_class(
    WrapTraits<CGAL::Iterator_range<I>> wrap, py::module& module) {
  using Wrap = WrapTraits<CGAL::Iterator_range<I>>;
  using Iterator_range = typename Wrap::cpp_type;

  typename Wrap::py_class pyclass =
      py::class_<Iterator_range>(module, wrap.name);
  pyclass.def(py::init<I, I>());
  pyclass.def("begin", &Iterator_range::begin);
  pyclass.def("end", &Iterator_range::end);
  pyclass.def("size", &Iterator_range::size);
  pyclass.def("empty", &Iterator_range::empty);
  pyclass.def("__iter__", [](Iterator_range& self) {
    return py::make_iterator(self.begin(), self.end());
  });

  return pyclass;
}

}  // namespace pyCGAL
