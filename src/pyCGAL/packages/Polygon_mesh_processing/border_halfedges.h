#pragma once

#include <pyCGAL/wrap/Polygon_mesh_processing/detail/border_halfedges.h>

#include "Polygon_mesh.h"

namespace pyCGAL::Polygon_mesh_processing {

using border_halfedges =
    pyCGAL::wrap::Polygon_mesh_processing::detail::border_halfedges<
        Polygon_mesh>;

}  // namespace pyCGAL::Polygon_mesh_processing
