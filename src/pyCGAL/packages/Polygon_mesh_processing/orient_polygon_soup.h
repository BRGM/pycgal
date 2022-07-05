#pragma once

#include <pyCGAL/wrap/Polygon_mesh_processing/detail/orient_polygon_soup.h>

#include "Polygon_soup.h"

namespace pyCGAL::Polygon_mesh_processing {

using orient_polygon_soup =
    pyCGAL::wrap::Polygon_mesh_processing::detail::orient_polygon_soup<
        Polygon_soup>;

}  // namespace pyCGAL::Polygon_mesh_processing
