#pragma once

#include <pyCGAL/wrap/Polygon_mesh_processing/detail/extrude_mesh.h>

#include "Polygon_mesh.h"

namespace pyCGAL::Polygon_mesh_processing {

using extrude_mesh =
    pyCGAL::wrap::Polygon_mesh_processing::detail::extrude_mesh<Polygon_mesh>;

}  // namespace pyCGAL::Polygon_mesh_processing
