#include <pyCGAL/wrap/PMP/stitch_borders.h>

#include "../Surface_mesh.h"

namespace pyCGAL::Epick::PMP {

void wrap_stitch_borders(py::module& module) {
  pyCGAL::wrap_algorithm(pyCGAL::PMP::stitch_borders<Surface_mesh>{}, module);
}

}  // namespace pyCGAL::Epick::PMP
