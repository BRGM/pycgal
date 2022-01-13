#include <pyCGAL/typedefs.h>
// generic definition of wrapping utilities
#include <pyCGAL/wrap/Mesh_2/Constrained_Delaunay_triangulation_2.h>
#include <pyCGAL/wrap/Mesh_2/Constrained_triangulation_2.h>
#include <pyCGAL/wrap/Mesh_2/Triangulation_2.h>
// per package definition of wrapped element
#include "Constrained_Delaunay_triangulations.h"

namespace pyCGAL::Mesh_2 {

void wrap_Constrained_Delaunay_triangulations(py::module& module) {
  using Triangulation_2 =
      typename Constrained_Delaunay_triangulation_2::Triangulation;
  static_assert(
      std::is_same_v<
          Triangulation_2,
          typename Constrained_Delaunay_triangulation_2_with_intersections::
              Triangulation>);
  using Constrained_triangulation_2 =
      typename Constrained_Delaunay_triangulation_2::Ctr;
  using Constrained_triangulation_2_with_intersections =
      typename Constrained_Delaunay_triangulation_2_with_intersections::Ctr;

  wrap_class(WrapTraits<Triangulation_2>{"Triangulation_2"}, module);
  wrap_class(
      WrapTraits<Constrained_triangulation_2>{"Constrained_triangulation_2"},
      module);
  wrap_class(
      WrapTraits<Constrained_triangulation_2_with_intersections>{
          "Constrained_triangulation_2_with_intersections"},
      module);
  wrap_class(
      WrapTraits<Constrained_Delaunay_triangulation_2>{
          "Constrained_Delaunay_triangulation_2"},
      module);
  wrap_class(
      WrapTraits<Constrained_Delaunay_triangulation_2_with_intersections>{
          "Constrained_Delaunay_triangulation_2_with_intersections"},
      module);
}

}  // namespace pyCGAL::Mesh_2
