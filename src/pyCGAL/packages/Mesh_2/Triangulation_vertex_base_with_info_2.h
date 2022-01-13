#pragma once

#include <CGAL/Triangulation_vertex_base_with_info_2.h>

#include "Kernel.h"
#include "Object_info.h"

namespace pyCGAL::Mesh_2 {

using Triangulation_vertex_base_with_info_2 =
    CGAL::Triangulation_vertex_base_with_info_2<Object_info, Kernel>;

}  // namespace pyCGAL::Mesh_2
