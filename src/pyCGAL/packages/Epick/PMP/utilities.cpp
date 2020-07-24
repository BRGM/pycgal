#include <pyCGAL/wrap/PMP/utilities.h>

#include "../Surface_mesh.h"

namespace pyCGAL::Epick::PMP {

void wrap_utilities(py::module& module) {
  pyCGAL::wrap_algorithm(pyCGAL::PMP::utilities<Surface_mesh>{}, module);
}

}  // namespace pyCGAL::Epick::PMP
