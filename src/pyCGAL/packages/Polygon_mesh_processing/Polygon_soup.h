#pragma once

#include <pyCGAL/addons/Polygon_mesh_processing/Polygon_soup.h>
#include <pyCGAL/dependencies.h>

#include "../Epick/Point_3.h"

namespace pyCGAL {

namespace Polygon_mesh_processing {

using Polygon_soup = pyCGAL::addons::Polygon_mesh_processing::Polygon_soup<
    pyCGAL::Epick::Point_3>;

}  // namespace Polygon_mesh_processing

template <>
struct Dependencies<Polygon_mesh_processing::Polygon_soup> {
  static std::string get() { return "pycgal.Epick"; }
};

}  //  namespace pyCGAL
