#pragma once

#include <CGAL/Mesh_criteria_3.h>

#include "Mesh_triangulation_3.h"

namespace pyCGAL {

namespace Mesh_3 {

using Mesh_criteria_3 = CGAL::Mesh_criteria_3<Mesh_triangulation_3>;

}  // namespace Mesh_3

}  //  namespace pyCGAL
