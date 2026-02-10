#pragma once

#include <CGAL/Mesh_complex_3_in_triangulation_3.h>
#include <pyCGAL/dependencies.h>

#include "Mesh_triangulation_3.h"
#include "Polyhedral_mesh_domain_with_features_3.h"

namespace pyCGAL {

namespace Mesh_3 {

using Mesh_complex_3_in_triangulation_3 =
    CGAL::Mesh_complex_3_in_triangulation_3<
        Mesh_triangulation_3,
        Polyhedral_mesh_domain_with_features_3::Corner_index,
        Polyhedral_mesh_domain_with_features_3::Curve_index>;

}  // namespace Mesh_3

template <>
struct Dependencies<Mesh_3::Mesh_complex_3_in_triangulation_3> {
  static std::string get() { return "pycgal.Epick;pycgal.Surface_mesh"; }
};

}  //  namespace pyCGAL
