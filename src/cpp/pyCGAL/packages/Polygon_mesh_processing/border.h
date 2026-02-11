#pragma once

#include <pyCGAL/wrap/Polygon_mesh_processing/detail/border.h>

#include "Polygon_mesh.h"

namespace pyCGAL::Polygon_mesh_processing {

using border =
    pyCGAL::wrap::Polygon_mesh_processing::detail::border<Polygon_mesh>;

}  // namespace pyCGAL::Polygon_mesh_processing
