#pragma once

#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Projection_traits_xy_3.h>
#include <pyCGAL/typedefs.h>
#include <pyCGAL/wrap/utils/generic_array.h>

#include "utils/as_arrays.h"

namespace pyCGAL {

namespace detail {
template <typename Kernel>
using Delaunay_terrain =
    CGAL::Delaunay_triangulation_2<CGAL::Projection_traits_xy_3<Kernel>>;
}

template <typename Kernel>
typename WrapTraits<detail::Delaunay_terrain<Kernel>>::py_class wrap_class(
    WrapTraits<detail::Delaunay_terrain<Kernel>>, py::module& module) {
  using Wrap = WrapTraits<detail::Delaunay_terrain<Kernel>>;
  using Delaunay_terrain = typename Wrap::cpp_type;
  using Point_3 = typename Kernel::Point_3;

  typename Wrap::py_class pyclass =
      py::class_<Delaunay_terrain>(module, "Delaunay_terrain")
          .def(py::init([](py::array_t<typename Point_3::FT,
                                       py::array::c_style |
                                           py::array::forcecast>& array) {
            auto points = pyCGAL::wrap::utils::make_point_array<Point_3>(array);
            return std::make_unique<Delaunay_terrain>(points.begin(),
                                                      points.end());
          }))
          .def("number_of_vertices", &Delaunay_terrain::number_of_vertices)
          .def("number_of_faces", &Delaunay_terrain::number_of_faces);

  module.def("as_arrays", [](const Delaunay_terrain& dt) {
    return wrap::utils::as_arrays(dt);
  });

  return pyclass;
}

}  // namespace pyCGAL
