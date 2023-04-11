#pragma once

#include <pyCGAL/wrap/Polygon_mesh_processing/detail/surface_intersection.h>

#include "Polygon_mesh.h"

namespace pyCGAL {

namespace Polygon_mesh_processing {

using surface_intersection =
    pyCGAL::wrap::Polygon_mesh_processing::detail::surface_intersection<
        Polygon_mesh>;

}  // namespace Polygon_mesh_processing

template <>
struct Dependencies<Polygon_mesh_processing::surface_intersection> {
  static std::string get() { return "pycgal.Epick"; }
};

}  //  namespace pyCGAL
