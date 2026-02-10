#pragma once

#include <pyCGAL/dependencies.h>
#include <pyCGAL/wrap/Surface_soup/extensions/Surface_soup.h>

#include "Extended_mesh.h"

namespace pyCGAL {

namespace Surface_soup {

using Surface_soup =
    pyCGAL::extensions::Surface_soup::Surface_soup<Extended_mesh>;

}  // namespace Surface_soup

template <>
struct Dependencies<Surface_soup::Surface_soup> {
  static std::string get() { return "pycgal.Epick;pycgal.Surface_mesh"; }
};

}  //  namespace pyCGAL
