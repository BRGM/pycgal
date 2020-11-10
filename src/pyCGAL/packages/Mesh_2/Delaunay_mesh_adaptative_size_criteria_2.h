#pragma once

#include <pyCGAL/addons/Delaunay_mesh_adaptative_size_criteria_2.h>

#include "Constrained_Delaunay_triangulation_2.h"

namespace pyCGAL {
namespace Mesh_2 {
using Delaunay_mesh_adaptative_size_criteria_2 =
    pyCGAL::addons::Delaunay_mesh_adaptative_size_criteria_2<
        Constrained_Delaunay_triangulation_2>;
}  // namespace Mesh_2
}  // namespace pyCGAL
