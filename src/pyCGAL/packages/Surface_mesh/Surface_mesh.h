#pragma once

#include <CGAL/Surface_mesh.h>
#include <pyCGAL/dependencies.h>

#include "../Epick/Point_3.h"

namespace pyCGAL {

namespace Surface_mesh {

using Surface_mesh = CGAL::Surface_mesh<pyCGAL::Epick::Point_3>;

}  // namespace Surface_mesh

template <>
struct Dependencies<Surface_mesh::Surface_mesh> {
  static std::string get() { return "pycgal.Epick"; }
};

}  //  namespace pyCGAL
