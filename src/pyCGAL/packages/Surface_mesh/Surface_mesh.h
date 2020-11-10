#pragma once

#include <CGAL/Surface_mesh.h>

#include "../Epick/Point_3.h"

namespace pyCGAL::Surface_mesh {
using Surface_mesh = CGAL::Surface_mesh<pyCGAL::Epick::Point_3>;
}  //  namespace pyCGAL::Surface_mesh
