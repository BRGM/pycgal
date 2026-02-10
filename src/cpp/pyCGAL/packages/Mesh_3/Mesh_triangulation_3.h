#pragma once

#include <CGAL/Mesh_triangulation_3.h>

#include "Polyhedral_mesh_domain_with_features_3.h"

namespace pyCGAL {

namespace Mesh_3 {

using Mesh_triangulation_3 =
    CGAL::Mesh_triangulation_3<Polyhedral_mesh_domain_with_features_3,
                               CGAL::Default, CGAL::Sequential_tag>::type;

}  // namespace Mesh_3

}  //  namespace pyCGAL
