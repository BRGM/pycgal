#pragma once

#include <pyCGAL/dependencies.h>
#include <pyCGAL/wrap/Polygon_mesh_processing/detail/polygon_soup_conversions.h>

#include "Polygon_mesh.h"
#include "Polygon_soup.h"

namespace pyCGAL {

namespace Polygon_mesh_processing {

using polygon_soup_conversions =
    pyCGAL::wrap::Polygon_mesh_processing::detail::polygon_soup_conversions<
        Polygon_soup, Polygon_mesh>;

}  // namespace Polygon_mesh_processing

template <>
struct Dependencies<Polygon_mesh_processing::polygon_soup_conversions> {
  static std::string get() { return "pycgal.Surface_mesh"; }
};

}  // namespace pyCGAL
