#pragma once

#include <CGAL/Triangulation_vertex_base_with_info_2.h>

#include "Kernel.h"
#include "Vertex_info.h"

namespace pyCGAL::Mesh_2 {

using Triangulation_vertex_base_with_info_2 =
    CGAL::Triangulation_vertex_base_with_info_2<Vertex_info, Kernel>;

}  // namespace pyCGAL::Mesh_2
