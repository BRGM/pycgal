#pragma once

#include <CGAL/Polygon_mesh_processing/intersection.h>
#include <pyCGAL/typedefs.h>
#include <pyCGAL/wrap/utils/named_parameters.h>
#include <pybind11/numpy.h>

#include "detail/surface_intersection.h"

namespace pyCGAL {

namespace detail = pyCGAL::wrap::Polygon_mesh_processing::detail;
namespace utils = pyCGAL::wrap::utils;

namespace {

template <typename Point>
auto polyline_to_array(const std::vector<Point>& polyline) {
  constexpr py::ssize_t dim = 3;
  static_assert(sizeof(Point) == dim * sizeof(double));
  auto result = py::array_t<double, py::array::c_style>{
      {(py::ssize_t)polyline.size(), dim}};
  copy(begin(polyline), end(polyline),
       reinterpret_cast<Point*>(result.mutable_data(0, 0)));
  return result;
}
}  // namespace

template <typename TriangleMesh>
void wrap_element(detail::surface_intersection<TriangleMesh>,
                  py::module& module) {
  module.def(
      "surface_intersection",
      [](TriangleMesh& tm1, TriangleMesh& tm2,
         const bool throw_on_self_intersection) {
        if (!CGAL::is_triangle_mesh(tm1) || !CGAL::is_triangle_mesh(tm2))
          throw std::runtime_error("Only triangle meshes can be corefined!");

        using Point = typename TriangleMesh::Point;
        using Polyline = std::vector<Point>;
        using Polylines = std::vector<Polyline>;

        Polylines polylines;

        CGAL::Polygon_mesh_processing::surface_intersection(
            tm1, tm2, back_inserter(polylines),
            CGAL::Polygon_mesh_processing::parameters::
                throw_on_self_intersection(throw_on_self_intersection));

        py::list result;
        for (auto&& polyline : polylines) {
          result.append(polyline_to_array(polyline));
        }

        return result;
      },
      py::arg("tm1").none(false), py::arg("tm2").none(false), py::kw_only(),
      py::arg("throw_on_self_intersection") = false);
}
}  // namespace pyCGAL
