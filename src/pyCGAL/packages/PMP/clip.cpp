#include <CGAL/Kernel_traits.h>
#include <CGAL/Plane_3.h>
#include <pyCGAL/wrap/PMP/clip.h>

#include "../Surface_mesh.h"

namespace pyCGAL::Epick::PMP {

void wrap_clip(py::module& module) {
  using Plane_3 = CGAL::Plane_3<
      typename CGAL::Kernel_traits<typename Surface_mesh::Point>::Kernel>;
  pyCGAL::wrap_algorithm(pyCGAL::PMP::clip<Surface_mesh, Plane_3>{}, module);
}

}  // namespace pyCGAL::Epick::PMP
