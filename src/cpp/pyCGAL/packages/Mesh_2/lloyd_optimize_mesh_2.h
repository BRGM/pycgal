#pragma once

#include <pyCGAL/wrap/Mesh_2/detail/lloyd_optimize_mesh_2.h>

#include "Constrained_Delaunay_triangulation_2.h"

namespace pyCGAL::Mesh_2 {

using lloyd_optimize_mesh_2 =
    pyCGAL::wrap::Mesh_2::detail::lloyd_optimize_mesh_2<
        Constrained_Delaunay_triangulation_2>;

}  // namespace pyCGAL::Mesh_2
