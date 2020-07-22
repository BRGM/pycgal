#pragma once

#include <CGAL/Surface_mesh.h>

#include "Point_3.h"

namespace pyCGAL {
namespace Epick {
using Surface_mesh = CGAL::Surface_mesh<Point_3>;
}  // namespace Epick
}  // namespace pyCGAL
