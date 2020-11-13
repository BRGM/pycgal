#pragma once

#include <pyCGAL/wrap/Polygon_mesh_processing/detail/split.h>

#include "Polygon_mesh.h"

namespace pyCGAL::Polygon_mesh_processing {

using split =
    pyCGAL::wrap::Polygon_mesh_processing::detail::split<Polygon_mesh>;

}  // namespace pyCGAL::Polygon_mesh_processing
