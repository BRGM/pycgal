#pragma once

#include <CGAL/Constrained_Delaunay_triangulation_2.h>

#include "Kernel.h"
#include "Triangulation_data_structure_2.h"

namespace pyCGAL::Mesh_2 {

using Constrained_Delaunay_triangulation_2 =
    CGAL::Constrained_Delaunay_triangulation_2<Kernel,
                                               Triangulation_data_structure_2>;

}  // namespace pyCGAL::Mesh_2
