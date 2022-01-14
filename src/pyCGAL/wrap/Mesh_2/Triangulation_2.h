#pragma once

#include <CGAL/Triangulation_2.h>
#include <pyCGAL/typedefs.h>

#include "detail/Triangulation_2.h"

namespace pyCGAL {
template <typename Traits, typename Tds>
typename WrapTraits<CGAL::Triangulation_2<Traits, Tds>>::py_class wrap_class(
    WrapTraits<CGAL::Triangulation_2<Traits, Tds>> wrap, py::module& module) {
  using Wrap = WrapTraits<CGAL::Triangulation_2<Traits, Tds>>;
  using Triangulation = typename Wrap::cpp_type;

  typename Wrap::py_class pyclass =
      py::class_<Triangulation>(module, wrap.name);
  pyclass.def(py::init<>());
  pyclass.def(py::init<const Triangulation&>());
  wrap::Mesh_2::detail::wrap_Triangulation_2<Triangulation>(pyclass);

  return pyclass;
}

}  // namespace pyCGAL
