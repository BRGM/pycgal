#pragma once

#include <pyCGAL/wrap/Polygon_mesh_processing/detail/corefine.h>

#include "Polygon_mesh.h"

namespace pyCGAL::Polygon_mesh_processing {

using corefine =
    pyCGAL::wrap::Polygon_mesh_processing::detail::corefine<Polygon_mesh>;

}  // namespace pyCGAL::Polygon_mesh_processing
