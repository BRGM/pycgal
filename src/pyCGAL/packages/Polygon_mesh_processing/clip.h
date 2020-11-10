#pragma once

#include <pyCGAL/wrap/Polygon_mesh_processing/detail/clip.h>

#include "Plane.h"
#include "Polygon_mesh.h"

namespace pyCGAL::Polygon_mesh_processing {

using clip =
    pyCGAL::wrap::Polygon_mesh_processing::detail::clip<Polygon_mesh, Plane>;

}  // namespace pyCGAL::Polygon_mesh_processing
