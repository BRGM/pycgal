#pragma once

#include <pyCGAL/wrap/LinearGeometryKernel/extensions/Polyline.h>
#include <pyCGAL/wrap/LinearGeometryKernel/extensions/Polylines.h>
#include <pyCGAL/wrap/utils/CGAL_FT.h>

namespace pyCGAL {
namespace Mesh_3 {
namespace utils {

namespace eLGK = extensions::LinearGeometryKernel;

template <typename Domain, typename pyClass>
void wrap_common_domain_api(pyClass& cls) {
  using Point = typename Domain::Point_3;

  cls.def("detect_borders", [](Domain& self) { self.detect_borders(); });
  cls.def(
      "detect_features",
      [](Domain& self, const wrap::utils::CGAL_FT<Point> angle) {
        self.detect_features(angle);
      },
      py::arg("angle") = 60);

  using Polyline = eLGK::Polyline<Point>;
  using Polylines = eLGK::Polylines<Polyline>;
  cls.def("add_features", [](Domain& self, const Polylines& features) {
    self.add_features(features.begin(), features.end());
  });
}

}  // namespace utils
}  // namespace Mesh_3
}  // namespace pyCGAL
