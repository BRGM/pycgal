#include <pyCGAL/wrap/PMP/isotropic_remeshing.h>

#include "../Surface_mesh.h"

namespace pyCGAL::Epick::PMP {

void wrap_isotropic_remeshing(py::module& module) {
  pyCGAL::wrap_algorithm(pyCGAL::PMP::isotropic_remeshing<Surface_mesh>{},
                         module);
}

}  // namespace pyCGAL::Epick::PMP
