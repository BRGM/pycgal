#pragma once

#include <CGAL/Delaunay_mesher_2.h>
#include <pyCGAL/typedefs.h>

namespace pyCGAL {

template <typename ConstrainedDelaunayTriangulation, typename Criteria>
typename WrapTraits<CGAL::Delaunay_mesher_2<ConstrainedDelaunayTriangulation,
                                            Criteria>>::py_class
wrap_class(WrapTraits<CGAL::Delaunay_mesher_2<ConstrainedDelaunayTriangulation,
                                              Criteria>>
               wrap,
           py::module& module) {
  using Wrap = WrapTraits<
      CGAL::Delaunay_mesher_2<ConstrainedDelaunayTriangulation, Criteria>>;
  using Mesher = typename Wrap::cpp_type;
  using CDT = ConstrainedDelaunayTriangulation;

  typename Wrap::py_class pyclass = py::class_<Mesher>(module, wrap.name);
  pyclass.def(py::init<CDT&>());
  pyclass.def("get_criteria", &Mesher::get_criteria);
  pyclass.def("set_criteria", &Mesher::set_criteria);
  pyclass.def("refine_mesh", &Mesher::refine_mesh);

  module.def("refine_Delaunay_mesh_2", [](CDT& cdt, const Criteria& criteria) {
    CGAL::refine_Delaunay_mesh_2(cdt, criteria);
  });

  return pyclass;
}

}  // namespace pyCGAL
