#pragma once

#include <CGAL/Triangulation_data_structure_2.h>

#include "Delaunay_mesh_face_base_2.h"
#include "Delaunay_mesh_vertex_base_2.h"

namespace pyCGAL::Mesh_2 {

using Triangulation_data_structure_2 =
    CGAL::Triangulation_data_structure_2<Delaunay_mesh_vertex_base_2,
                                         Delaunay_mesh_face_base_2>;

}  // namespace pyCGAL::Mesh_2
