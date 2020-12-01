#pragma once

#include <pyCGAL/wrap/BGL/detail/Euler_operations.h>

#include "Surface_mesh.h"

namespace pyCGAL::Surface_mesh {

using Euler_operations =
    pyCGAL::wrap::BGL::detail::Euler_operations<Surface_mesh>;

}  //  namespace pyCGAL::Surface_mesh
