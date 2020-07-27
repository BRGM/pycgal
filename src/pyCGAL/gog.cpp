#include <CGAL/Random.h>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
namespace py = pybind11;

PYBIND11_MODULE(gog, module) {
  using State = CGAL::Random::State;
  py::class_<State>(module, "State");

  using Random = CGAL::Random;
  py::class_<Random>(module, "Random")
      .def(py::init<>())
      .def(py::init<unsigned int>())
      .def("get_bool", &Random::get_bool)
      //.def("get_bits", &Random::get_bits) // template method
      .def("get_int", &Random::get_int)
      .def("get_double", &Random::get_double)
      .def("save_state", &Random::save_state)
      .def("restore_state", &Random::restore_state)
      .def(py::self == py::self);

  module.def("get_default_random", &CGAL::get_default_random);
  module.def("set_default_random",
             [](const Random& random) { CGAL::get_default_random() = random; });
}
