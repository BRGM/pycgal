#pragma once

#include <CGAL/Delaunay_mesh_face_base_2.h>

#include "Kernel.h"

namespace pyCGAL::Mesh_2 {

using Delaunay_mesh_face_base_2 = CGAL::Delaunay_mesh_face_base_2<Kernel>;

}  // namespace pyCGAL::Mesh_2
