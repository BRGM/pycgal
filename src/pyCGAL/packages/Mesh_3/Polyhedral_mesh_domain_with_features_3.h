#pragma once

#include <CGAL/Polyhedral_mesh_domain_with_features_3.h>
#include <pyCGAL/dependencies.h>

#include "Kernel.h"

namespace pyCGAL {

namespace Mesh_3 {

using Polyhedral_mesh_domain_with_features_3 =
    CGAL::Polyhedral_mesh_domain_with_features_3<Kernel>;

}  // namespace Mesh_3

template <>
struct Dependencies<Mesh_3::Polyhedral_mesh_domain_with_features_3> {
  static std::string get() { return "pycgal.Epick;pycgal.Surface_mesh"; }
};

}  //  namespace pyCGAL
