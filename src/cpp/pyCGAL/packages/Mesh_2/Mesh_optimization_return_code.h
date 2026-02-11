#pragma once

#include <CGAL/Mesh_optimization_return_code.h>
#include <pyCGAL/typedefs.h>

namespace pyCGAL::Mesh_2 {

using Mesh_optimization_return_code =
    pyCGAL::CGALEnum<CGAL::Mesh_optimization_return_code>;

}  // namespace pyCGAL::Mesh_2
