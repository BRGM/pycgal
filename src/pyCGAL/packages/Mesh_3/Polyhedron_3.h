#pragma once

#include <CGAL/Mesh_polyhedron_3.h>
#include <pyCGAL/dependencies.h>

#include "Kernel.h"

namespace pyCGAL {

namespace Mesh_3 {

using Polyhedron_3 = CGAL::Mesh_polyhedron_3<Kernel>::type;

}  // namespace Mesh_3

template <>
struct Dependencies<Mesh_3::Polyhedron_3> {
  static std::string get() { return "pycgal.Epick"; }
};

}  //  namespace pyCGAL
