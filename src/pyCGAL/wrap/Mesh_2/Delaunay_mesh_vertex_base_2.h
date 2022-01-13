#pragma once

#include <CGAL/Delaunay_mesh_vertex_base_2.h>
#include <pyCGAL/typedefs.h>

namespace pyCGAL {

template <typename Kernel, typename Vb>
typename WrapTraits<CGAL::Delaunay_mesh_vertex_base_2<Kernel, Vb>>::py_class
wrap_class(WrapTraits<CGAL::Delaunay_mesh_vertex_base_2<Kernel, Vb>>,
           py::module& module) {
  using Wrap = WrapTraits<CGAL::Delaunay_mesh_vertex_base_2<Kernel, Vb>>;

  typename Wrap::py_class pyclass = py::class_<typename Wrap::cpp_type>(
      module, "Delaunay_mesh_vertex_base_2");
  return pyclass;
}

}  // namespace pyCGAL
