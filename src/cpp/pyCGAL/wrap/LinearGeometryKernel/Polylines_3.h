#pragma once

#include <pyCGAL/typedefs.h>
#include <pyCGAL/wrap/utils/polyline_to_array.h>

#include "extensions/Polylines.h"

namespace pyCGAL {

namespace eLGK = extensions::LinearGeometryKernel;
namespace utils = wrap::utils;

template <typename Polyline>
typename WrapTraits<eLGK::Polylines<Polyline>>::py_class wrap_class(
    WrapTraits<eLGK::Polylines<Polyline>> wrap, py::module& module) {
  using Polylines = eLGK::Polylines<Polyline>;
  auto cls = py::class_<Polylines>(module, wrap.name);
  cls.def("__len__", &Polylines::size);
  cls.def(
      "__iter__",
      [](Polylines& self) {
        return py::make_iterator(self.begin(), self.end());
      },
      py::keep_alive<0, 1>());
  cls.def("as_list", [](const Polylines& self) {
    return utils::polylines_to_list(self.polylines);
  });
  return cls;
}

}  // namespace pyCGAL
