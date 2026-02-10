#pragma once

#include <CGAL/Triangulation_face_base_with_info_2.h>

#include "detail/Element_with_info.h"

namespace pyCGAL {

template <typename Info, typename Traits, typename Vb>
typename WrapTraits<
    CGAL::Triangulation_face_base_with_info_2<Info, Traits, Vb>>::py_class
wrap_class(
    WrapTraits<CGAL::Triangulation_face_base_with_info_2<Info, Traits, Vb>>,
    py::module& module) {
  using Wrap =
      WrapTraits<CGAL::Triangulation_face_base_with_info_2<Info, Traits, Vb>>;

  return wrap::Mesh_2::detail::wrap_element_with_info<Wrap>(
      module, "Triangulation_face_base_with_info_2");
}

}  // namespace pyCGAL
