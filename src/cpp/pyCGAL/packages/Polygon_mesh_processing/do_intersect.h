#pragma once

#include <pyCGAL/wrap/Polygon_mesh_processing/detail/do_intersect.h>

#include "Polygon_mesh.h"

namespace pyCGAL::Polygon_mesh_processing {

using do_intersect =
    pyCGAL::wrap::Polygon_mesh_processing::detail::do_intersect<Polygon_mesh>;

}  // namespace pyCGAL::Polygon_mesh_processing
