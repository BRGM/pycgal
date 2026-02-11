#pragma once

#include <pyCGAL/wrap/Polygon_mesh_processing/detail/reverse_face_orientations.h>

#include "Polygon_mesh.h"

namespace pyCGAL::Polygon_mesh_processing {

using reverse_face_orientations =
    pyCGAL::wrap::Polygon_mesh_processing::detail::reverse_face_orientations<
        Polygon_mesh>;

}  // namespace pyCGAL::Polygon_mesh_processing
