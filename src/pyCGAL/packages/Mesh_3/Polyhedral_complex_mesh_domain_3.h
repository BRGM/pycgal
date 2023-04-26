#pragma once

#include <CGAL/Polyhedral_complex_mesh_domain_3.h>
#include <pyCGAL/dependencies.h>

#include "Kernel.h"

namespace pyCGAL {

namespace Mesh_3 {

using Polyhedral_complex_mesh_domain_3 =
    CGAL::Polyhedral_complex_mesh_domain_3<Kernel>;

}  // namespace Mesh_3

template <>
struct Dependencies<Mesh_3::Polyhedral_complex_mesh_domain_3> {
  static std::string get() { return "pycgal.Epick;pycgal.Surface_mesh"; }
};

}  //  namespace pyCGAL
