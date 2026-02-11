#pragma once

#include <pyCGAL/wrap/Polygon_mesh_processing/detail/smooth_shape.h>

#include "Polygon_mesh.h"

namespace pyCGAL::Polygon_mesh_processing {

using smooth_shape =
    pyCGAL::wrap::Polygon_mesh_processing::detail::smooth_shape<Polygon_mesh>;

}  // namespace pyCGAL::Polygon_mesh_processing
