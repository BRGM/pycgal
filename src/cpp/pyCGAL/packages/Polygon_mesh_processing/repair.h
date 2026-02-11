#pragma once

#include <pyCGAL/wrap/Polygon_mesh_processing/detail/repair.h>

#include "Polygon_mesh.h"

namespace pyCGAL::Polygon_mesh_processing {

using repair =
    pyCGAL::wrap::Polygon_mesh_processing::detail::repair<Polygon_mesh>;

}  // namespace pyCGAL::Polygon_mesh_processing
