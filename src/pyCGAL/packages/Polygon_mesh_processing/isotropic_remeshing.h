#pragma once

#include <pyCGAL/wrap/Polygon_mesh_processing/detail/isotropic_remeshing.h>

#include "Polygon_mesh.h"

namespace pyCGAL::Polygon_mesh_processing {

using isotropic_remeshing =
    pyCGAL::wrap::Polygon_mesh_processing::detail::isotropic_remeshing<
        Polygon_mesh>;

}  // namespace pyCGAL::Polygon_mesh_processing
