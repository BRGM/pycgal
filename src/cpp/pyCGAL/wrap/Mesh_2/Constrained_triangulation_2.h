#pragma once

#include <CGAL/Constrained_triangulation_2.h>
#include <pyCGAL/typedefs.h>

#include "detail/Constrained_triangulation_2.h"
#include "detail/Triangulation_2.h"

namespace pyCGAL {

template <typename Traits, typename Tds, typename Itag>
typename WrapTraits<
    CGAL::Constrained_triangulation_2<Traits, Tds, Itag>>::py_class
wrap_class(
    WrapTraits<CGAL::Constrained_triangulation_2<Traits, Tds, Itag>> wrap,
    py::module& module) {
  using Wrap = WrapTraits<CGAL::Constrained_triangulation_2<Traits, Tds, Itag>>;
  using Ct = typename Wrap::cpp_type;

  using Vertex_handle = typename Ct::Vertex_handle;
  using Face_handle = typename Ct::Face_handle;
  using Point = typename Ct::Point;

  // for a strange reason the following does not work...
  // we call this the *inheritance bug*
  // it is probably linked to the fact that some methods are reimplemented
  // using Triangulation = CGAL::Triangulation_2<Traits, Tds>;
  // static_assert(std::is_base_of_v<Triangulation, Ct>);
  // typename Wrap::py_class pyclass =
  //    py::class_<Ct, Triangulation>(module, wrap.name);

  typename Wrap::py_class pyclass = py::class_<Ct>(module, wrap.name);
  pyclass.def(py::init<>());
  pyclass.def(py::init<const Ct&>());
  // because of the *inheritance bug* we factorized member wrapping
  wrap::Mesh_2::detail::wrap_Triangulation_2<Ct>(pyclass);
  wrap::Mesh_2::detail::wrap_Constrained_triangulation_2<Ct>(pyclass);

  return pyclass;
}

}  // namespace pyCGAL
