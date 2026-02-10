#pragma once

#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Projection_traits_xy_3.h>

#include "Kernel.h"

namespace pyCGAL::Mesh_2 {

using Delaunay_terrain =
    CGAL::Delaunay_triangulation_2<CGAL::Projection_traits_xy_3<Kernel>>;

}  // namespace pyCGAL::Mesh_2
