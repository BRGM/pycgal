#pragma once

#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <pyCGAL/typedefs.h>

namespace pyCGAL {

template <typename Info, typename Traits, typename Vb>
typename WrapTraits<
    CGAL::Triangulation_vertex_base_with_info_2<Info, Traits, Vb>>::py_class
wrap_class(
    WrapTraits<CGAL::Triangulation_vertex_base_with_info_2<Info, Traits, Vb>>,
    py::module& module) {
  using Wrap =
      WrapTraits<CGAL::Triangulation_vertex_base_with_info_2<Info, Traits, Vb>>;
  using VBI = typename Wrap::cpp_type;

  typename Wrap::py_class pyclass =
      py::class_<VBI>(module, "Triangulation_vertex_base_with_info_2");
  pyclass.def(py::init<>());
  pyclass.def_property("info", py::overload_cast<>(&VBI::info),
                       [](VBI& self, py::object& obj) { self.info() = obj; });
  return pyclass;
}

}  // namespace pyCGAL
