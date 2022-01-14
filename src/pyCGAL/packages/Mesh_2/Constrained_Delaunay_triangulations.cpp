#include <pyCGAL/typedefs.h>
// generic definition of wrapping utilities
#include <pyCGAL/wrap/Mesh_2/Constrained_Delaunay_triangulation_2.h>
#include <pyCGAL/wrap/Mesh_2/Constrained_triangulation_2.h>
#include <pyCGAL/wrap/Mesh_2/Triangulation_2.h>
#include <pyCGAL/wrap/core/Iterator_range.h>
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
  using Vertex_handle = typename Triangulation_2::Vertex_handle;
  using Face_handle = typename Triangulation_2::Face_handle;
  using Vertex = typename Triangulation_2::Vertex;
  using Face = typename Triangulation_2::Face;
  using Edge = typename Triangulation_2::Edge;
  using Points = typename Triangulation_2::Points;
  using Constrained_triangulation_2 =
      typename Constrained_Delaunay_triangulation_2::Ctr;
  using Constrained_triangulation_2_with_intersections =
      typename Constrained_Delaunay_triangulation_2_with_intersections::Ctr;

  py::class_<Vertex_handle>(module, "Vertex_handle")
      .def("point", [](Vertex_handle& self) { return *self; });
  py::class_<Face>(module, "Face")
      .def("vertex", &Face::vertex)
      .def("vertices", [](const Face& self) {
        return py::make_tuple(self.vertex(0), self.vertex(1), self.vertex(2));
      });
  py::class_<Face_handle>(module, "Face_handle");
  py::class_<Edge>(module, "Edge").def("vertices", [](const Edge& self) {
    auto [fh, i] = self;
    return py::make_tuple(fh->vertex((i + 1) % 3), fh->vertex((i + 2) % 3));
  });
  wrap_class(WrapTraits<Points>{"Points"}, module);
  wrap_class(WrapTraits<Triangulation_2>{"Triangulation_2"}, module);

  wrap_class(
      WrapTraits<typename Constrained_triangulation_2::Constrained_edges>{
          "Constrained_edges"},
      module);
  wrap_class(
      WrapTraits<typename Constrained_triangulation_2_with_intersections::
                     Constrained_edges>{"Constrained_edges_with_intersections"},
      module);
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
