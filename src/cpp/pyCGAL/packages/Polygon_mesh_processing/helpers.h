#pragma once

#include <pyCGAL/wrap/Polygon_mesh_processing/detail/helpers.h>

#include "Polygon_mesh.h"

namespace pyCGAL::Polygon_mesh_processing {

using helpers =
    pyCGAL::wrap::Polygon_mesh_processing::detail::helpers<Polygon_mesh>;

}  // namespace pyCGAL::Polygon_mesh_processing
