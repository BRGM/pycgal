#pragma once

#include <CGAL/Constrained_Delaunay_triangulation_2.h>

#include "Intersection_tags.h"
#include "Kernel.h"
#include "Triangulation_data_structure_2.h"

namespace pyCGAL::Mesh_2 {

using Constrained_Delaunay_triangulation_2_with_intersections =
    CGAL::Constrained_Delaunay_triangulation_2<
        Kernel, Triangulation_data_structure_2,
        typename Intersection_tag<Kernel>::type>;

}  // namespace pyCGAL::Mesh_2
