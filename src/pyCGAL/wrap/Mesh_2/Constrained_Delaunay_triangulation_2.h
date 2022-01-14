#pragma once

#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <pyCGAL/typedefs.h>

#include "detail/Constrained_triangulation_2.h"
#include "detail/Triangulation_2.h"

namespace pyCGAL {
template <typename Traits, typename Tds, typename Itag>
typename WrapTraits<
    CGAL::Constrained_Delaunay_triangulation_2<Traits, Tds, Itag>>::py_class
wrap_class(
    WrapTraits<CGAL::Constrained_Delaunay_triangulation_2<Traits, Tds, Itag>>
        wrap,
    py::module& module) {
  using Wrap =
      WrapTraits<CGAL::Constrained_Delaunay_triangulation_2<Traits, Tds, Itag>>;
  using CDt = typename Wrap::cpp_type;

  // for a strange reason the following does not work...
  // we call this the *inheritance bug*
  // it is probably linked to the fact that some methods are reimplemented
  // using Ct = CGAL::Constrained_triangulation_2<Traits, Tds, Itag>;
  // static_assert(std::is_base_of_v<Ct, CDt>);
  // typename Wrap::py_class pyclass = py::class_<CDt, Ct>(module, wrap.name);

  typename Wrap::py_class pyclass = py::class_<CDt>(module, wrap.name);
  pyclass.def(py::init<>());
  pyclass.def(py::init<const CDt&>());
  // because of the *inheritance bug* we factorized member wrapping
  wrap::Mesh_2::detail::wrap_Triangulation_2<CDt>(pyclass);
  wrap::Mesh_2::detail::wrap_Constrained_triangulation_2<CDt>(pyclass);

  return pyclass;
}

}  // namespace pyCGAL
