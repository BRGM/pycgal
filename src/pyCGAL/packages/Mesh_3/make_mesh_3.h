#pragma once

#include <CGAL/make_mesh_3.h>

#include "Mesh_complex_3_in_triangulation_3.h"
#include "Polyhedral_mesh_domain_with_features_3.h"

namespace pyCGAL::Mesh_3 {

using make_mesh_3 = pyCGAL::wrap::Mesh_3::detail::make_mesh_3<
    Polyhedral_mesh_domain_with_features_3, Mesh_complex_3_in_triangulation_3>;

}  // namespace pyCGAL::Mesh_3
