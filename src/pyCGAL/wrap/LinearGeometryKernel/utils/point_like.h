#pragma once

#include <CGAL/Dimension.h>
#include <pybind11/numpy.h>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>

#include "pyCGAL/wrap/utils/CGAL_FT.h"

namespace py = pybind11;

namespace pyCGAL::wrap::utils {

template <int dim>
struct dim_specific;

template <>
struct dim_specific<2> {
  template <typename PointLike, typename PyClass>
  static void wrap(PyClass& pyclass) {
    typedef CGAL_FT<PointLike> FT;
    pyclass.def(py::init<double, double>());
    pyclass.def_property_readonly("x", &PointLike::x);
    pyclass.def_property_readonly("y", &PointLike::y);
    pyclass.def(py::init(
        [](py::array_t<FT, py::array::c_style | py::array::forcecast> a) {
          assert(a.ndim() == 1);
          assert(a.size() == 2);
          auto x = a.template unchecked<1>();
          return std::make_unique<PointLike>(x(0), x(1));
        }));
  }
};

template <>
struct dim_specific<3> {
  template <typename PointLike, typename PyClass>
  static void wrap(PyClass& pyclass) {
    typedef CGAL_FT<PointLike> FT;
    pyclass.def(py::init<double, double, double>());
    pyclass.def_property_readonly("x", &PointLike::x);
    pyclass.def_property_readonly("y", &PointLike::y);
    pyclass.def_property_readonly("z", &PointLike::z);
    pyclass.def(py::init(
        [](py::array_t<FT, py::array::c_style | py::array::forcecast> a) {
          assert(a.ndim() == 1);
          assert(a.size() == 3);
          auto x = a.template unchecked<1>();
          return std::make_unique<PointLike>(x(0), x(1), x(2));
        }));
  }
};

template <typename PointLike, typename VectorLike>
auto wrap_point_like(py::module& module, const char* name) {
  py::class_<PointLike> pyclass{module, name, py::buffer_protocol()};

  pyclass.def_buffer([](PointLike& self) {
    typedef CGAL_FT<PointLike> FT;
    constexpr auto dim = CGAL::Ambient_dimension<PointLike>::value;
    static_assert(dim * sizeof(FT) == sizeof(PointLike), "Inconsistent sizes!");
    return py::buffer_info(reinterpret_cast<FT*>(&self), dim);
  });

  pyclass.def(py::init<const PointLike&>());

  pyclass.def("dimension", &PointLike::dimension);
  pyclass.def("__getitem__", &PointLike::operator[]);

  pyclass.def("__str__", [](const PointLike& self) {
    std::ostringstream os;
    os << "CGAL " << self;
    return os.str();
  });

  constexpr auto dim = CGAL::Ambient_dimension<PointLike>::value;
  dim_specific<dim>::template wrap<PointLike>(pyclass);

  pyclass.def(py::self - py::self);
  pyclass.def(py::self + VectorLike());
  pyclass.def(py::self - VectorLike());
  pyclass.def(py::self == py::self);
  pyclass.def(py::self != py::self);
  pyclass.def(py::self += VectorLike());
  pyclass.def(py::self -= VectorLike());

  return pyclass;
}

}  // namespace pyCGAL::wrap::utils
