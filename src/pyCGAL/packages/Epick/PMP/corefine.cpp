#include <CGAL/Kernel_traits.h>
#include <pyCGAL/wrap/PMP/corefine.h>

#include "../Surface_mesh.h"

namespace pyCGAL::Epick::PMP {

void wrap_corefine(py::module& module) {
  pyCGAL::wrap_algorithm(pyCGAL::PMP::corefine<Surface_mesh, Surface_mesh>{},
                         module);
}

}  // namespace pyCGAL::Epick::PMP
