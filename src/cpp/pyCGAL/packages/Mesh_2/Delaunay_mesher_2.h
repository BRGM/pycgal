#pragma once

#include <CGAL/Delaunay_mesher_2.h>

#include "Constrained_Delaunay_triangulation_2.h"
#include "Delaunay_mesh_adaptative_size_criteria_2.h"

namespace pyCGAL::Mesh_2 {

using Delaunay_mesher_2 =
    CGAL::Delaunay_mesher_2<Constrained_Delaunay_triangulation_2,
                            Delaunay_mesh_adaptative_size_criteria_2>;

}  // namespace pyCGAL::Mesh_2
