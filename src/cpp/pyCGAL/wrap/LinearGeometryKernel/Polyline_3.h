#pragma once

#include <pyCGAL/typedefs.h>
#include <pyCGAL/wrap/utils/polyline_to_array.h>

#include "extensions/Polyline.h"

namespace pyCGAL {

namespace eLGK = extensions::LinearGeometryKernel;
namespace utils = wrap::utils;

template <typename Point>
typename WrapTraits<eLGK::Polyline<Point>>::py_class wrap_class(
    WrapTraits<eLGK::Polyline<Point>> wrap, py::module& module) {
  using Polyline = eLGK::Polyline<Point>;
  auto cls = py::class_<Polyline>(module, wrap.name);
  cls.def("__len__", &Polyline::size);
  cls.def(
      "__iter__",
      [](Polyline& self) {
        return py::make_iterator(self.begin(), self.end());
      },
      py::keep_alive<0, 1>());
  cls.def("as_array", [](const Polyline& self) {
    return utils::polyline_to_array(self.points);
  });
  return cls;
}

}  // namespace pyCGAL
