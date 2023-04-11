#pragma once

#include <pyCGAL/dependencies.h>
#include <pyCGAL/wrap/Surface_soup/extensions/Extended_mesh.h>

#include "../Surface_mesh/Surface_mesh.h"

namespace pyCGAL {

namespace Surface_soup {

using Extended_mesh = pyCGAL::extensions::Surface_soup::Extended_mesh<
    pyCGAL::Surface_mesh::Surface_mesh>;

}  // namespace Surface_soup

template <>
struct Dependencies<Surface_soup::Extended_mesh> {
  static std::string get() { return "pycgal.Surface_mesh"; }
};

}  //  namespace pyCGAL
