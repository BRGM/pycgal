#pragma once

#include <CGAL/Random.h>
#include <pyCGAL/typedefs.h>
#include <pybind11/operators.h>

namespace pyCGAL {

template <>
typename WrapTraits<CGAL::Random>::py_class wrap_class(WrapTraits<CGAL::Random>,
                                                       py::module& module) {
  using Wrap = WrapTraits<CGAL::Random>;
  using Random = typename Wrap::cpp_type;

  typename Wrap::py_class pyclass = py::class_<Random>(module, "Random");
  pyclass.def(py::init<>());
  pyclass.def(py::init<unsigned int>());
  pyclass.def("get_bool", &Random::get_bool);
  // pyclass.def("get_bits", &Random::get_bits); // template method
  pyclass.def("get_int", &Random::get_int);
  pyclass.def("get_double", &Random::get_double);
  pyclass.def("save_state", &Random::save_state);
  pyclass.def("restore_state", &Random::restore_state);
  pyclass.def(py::self == py::self);

  module.def("get_default_random", &CGAL::get_default_random);
  module.def("set_default_random",
             [](const Random& random) { CGAL::get_default_random() = random; });

  return pyclass;
}

}  // namespace pyCGAL
