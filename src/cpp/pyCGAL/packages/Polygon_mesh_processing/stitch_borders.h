#pragma once

#include <pyCGAL/wrap/Polygon_mesh_processing/detail/stitch_borders.h>

#include "Polygon_mesh.h"

namespace pyCGAL::Polygon_mesh_processing {

using stitch_borders =
    pyCGAL::wrap::Polygon_mesh_processing::detail::stitch_borders<Polygon_mesh>;

}  // namespace pyCGAL::Polygon_mesh_processing
