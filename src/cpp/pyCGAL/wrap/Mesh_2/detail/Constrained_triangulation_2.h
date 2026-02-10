#pragma once

#include <pyCGAL/typedefs.h>
#include <pyCGAL/wrap/core/Iterator_range.h>

#include "../utils/as_arrays.h"

namespace pyCGAL::wrap::Mesh_2::detail {

template <typename Triangulation, typename Py_class>
Py_class& wrap_Constrained_triangulation_2(Py_class& pyclass) {
  using Vertex_handle = typename Triangulation::Vertex_handle;
  using Point = typename Triangulation::Point;
  using Constrained_edges = typename Triangulation::Constrained_edges;

  pyclass.def("insert", [](Triangulation& self, const Point& P) {
    return self.insert(P);
  });
  pyclass.def("insert_constraint",
              [](Triangulation& self, const Point& P, const Point& Q) {
                return self.insert_constraint(P, Q);
              });
  pyclass.def("insert_constraint",
              [](Triangulation& self, Vertex_handle vP, Vertex_handle vQ) {
                return self.insert_constraint(vP, vQ);
              });
  pyclass.def("constrained_edges", &Triangulation::constrained_edges);
  pyclass.def("as_arrays_with_constraints",
              &wrap::utils::as_arrays_with_constraints<Triangulation>);
  return pyclass;
}

}  // namespace pyCGAL::wrap::Mesh_2::detail
