#pragma once

#include <pyCGAL/wrap/Polygon_mesh_processing/detail/surface_intersection.h>

#include "Polygon_mesh.h"

namespace pyCGAL::Polygon_mesh_processing {

using surface_intersection =
    pyCGAL::wrap::Polygon_mesh_processing::detail::surface_intersection<
        Polygon_mesh>;

}  // namespace pyCGAL::Polygon_mesh_processing
