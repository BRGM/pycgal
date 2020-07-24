#include <pyCGAL/wrap/PMP/triangulate_faces.h>

#include "../Surface_mesh.h"

namespace pyCGAL::Epick::PMP {

void wrap_triangulate_faces(py::module& module) {
  pyCGAL::wrap_algorithm(pyCGAL::PMP::triangulate_faces<Surface_mesh>{},
                         module);
}

}  // namespace pyCGAL::Epick::PMP
