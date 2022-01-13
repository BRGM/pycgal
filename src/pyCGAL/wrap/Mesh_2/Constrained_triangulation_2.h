#pragma once

#include <CGAL/Constrained_triangulation_2.h>
#include <pyCGAL/typedefs.h>

#include "utils/as_arrays.h"

namespace pyCGAL {
template <typename Traits, typename Tds, typename Itag>
typename WrapTraits<
    CGAL::Constrained_triangulation_2<Traits, Tds, Itag>>::py_class
wrap_class(
    WrapTraits<CGAL::Constrained_triangulation_2<Traits, Tds, Itag>> wrap,
    py::module& module) {
  using Wrap = WrapTraits<CGAL::Constrained_triangulation_2<Traits, Tds, Itag>>;
  using Ct = typename Wrap::cpp_type;

  // using Triangulation = CGAL::Triangulation_2<Traits, Tds>;
  // static_assert(std::is_base_of_v<Triangulation, Ct>);

  using Vertex_handle = typename Ct::Vertex_handle;
  using Face_handle = typename Ct::Face_handle;
  using Point = typename Ct::Point;

  // typename Wrap::py_class pyclass =
  //    py::class_<Ct, Triangulation>(module, wrap.name);
  typename Wrap::py_class pyclass = py::class_<Ct>(module, wrap.name);
  pyclass.def(py::init<>());
  pyclass.def(py::init<const Ct&>());
  // Triangulation_2 members
  pyclass.def("clear", &Ct::clear);
  pyclass.def("dimension", &Ct::dimension);
  pyclass.def("number_of_vertices", &Ct::number_of_vertices);
  pyclass.def("number_of_faces", &Ct::number_of_faces);
  // Constrained_triangulation_2 members
  pyclass.def("insert",
              [](Ct& self, const Point& P) { return self.insert(P); });
  pyclass.def("insert_constraint",
              [](Ct& self, const Point& P, const Point& Q) {
                return self.insert_constraint(P, Q);
              });
  pyclass.def("insert_constraint",
              [](Ct& self, Vertex_handle vP, Vertex_handle vQ) {
                return self.insert_constraint(vP, vQ);
              });

  return pyclass;
}

}  // namespace pyCGAL
