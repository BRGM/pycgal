#pragma once

#include <pyCGAL/wrap/Polygon_mesh_processing/detail/triangulate_faces.h>

#include "Polygon_mesh.h"

namespace pyCGAL::Polygon_mesh_processing {

using triangulate_faces =
    pyCGAL::wrap::Polygon_mesh_processing::detail::triangulate_faces<
        Polygon_mesh>;

}  // namespace pyCGAL::Polygon_mesh_processing
