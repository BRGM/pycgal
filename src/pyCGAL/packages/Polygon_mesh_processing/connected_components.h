#pragma once

#include <pyCGAL/wrap/Polygon_mesh_processing/detail/connected_components.h>

#include "Polygon_mesh.h"

namespace pyCGAL::Polygon_mesh_processing {

using connected_components =
    pyCGAL::wrap::Polygon_mesh_processing::detail::connected_components<
        Polygon_mesh>;

}  // namespace pyCGAL::Polygon_mesh_processing
