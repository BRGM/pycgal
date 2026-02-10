#pragma once

#include <pyCGAL/wrap/Polygon_mesh_processing/detail/remesh.h>

#include "Polygon_mesh.h"

namespace pyCGAL::Polygon_mesh_processing {

using remesh =
    pyCGAL::wrap::Polygon_mesh_processing::detail::remesh<Polygon_mesh>;

}  // namespace pyCGAL::Polygon_mesh_processing
