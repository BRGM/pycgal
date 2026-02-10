#pragma once

#include <CGAL/Delaunay_mesher_2.h>

#include "Constrained_Delaunay_triangulation_2_with_intersections.h"
#include "Delaunay_mesh_adaptative_size_criteria_2.h"

namespace pyCGAL::Mesh_2 {

using Delaunay_mesher_2_with_intersections = CGAL::Delaunay_mesher_2<
    Constrained_Delaunay_triangulation_2_with_intersections,
    Delaunay_mesh_adaptative_size_criteria_2>;

}  // namespace pyCGAL::Mesh_2
