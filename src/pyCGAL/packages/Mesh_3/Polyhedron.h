#pragma once

#include <CGAL/Mesh_polyhedron_3.h>
#include <pyCGAL/dependencies.h>

#include "../Epick/Kernel.h"

namespace pyCGAL {

namespace Mesh_3 {

using Polyhedron = CGAL::Mesh_polyhedron_3<pyCGAL::Epick::Kernel>::type;

}  // namespace Mesh_3

template <>
struct Dependencies<Mesh_3::Polyhedron> {
  static std::string get() { return "pycgal.Epick"; }
};

}  //  namespace pyCGAL
