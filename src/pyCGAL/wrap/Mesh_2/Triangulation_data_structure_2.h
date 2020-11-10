#pragma once

#include <CGAL/Triangulation_data_structure_2.h>
#include <pyCGAL/typedefs.h>

namespace pyCGAL {

template <typename Vb, typename Fb>
typename WrapTraits<CGAL::Triangulation_data_structure_2<Vb, Fb>>::py_class
wrap_class(WrapTraits<CGAL::Triangulation_data_structure_2<Vb, Fb>>,
           py::module& module) {
  using Wrap = WrapTraits<CGAL::Triangulation_data_structure_2<Vb, Fb>>;

  typename Wrap::py_class pyclass = py::class_<typename Wrap::cpp_type>(
      module, "Triangulation_data_structure_2");
  return pyclass;
}

}  // namespace pyCGAL
