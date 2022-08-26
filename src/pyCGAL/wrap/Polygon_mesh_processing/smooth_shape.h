#pragma once

#include <CGAL/Polygon_mesh_processing/smooth_shape.h>
#include <pyCGAL/typedefs.h>

#include "detail/smooth_shape.h"
#include "utils/parameter_helpers.h"
#include "utils/property_helpers.h"

namespace pyCGAL {

namespace implementation {

namespace utils = pyCGAL::wrap::utils;

template <typename PolygonMesh, typename FaceRange>
void smooth_shape(PolygonMesh& mesh, const double time, py::object face_group,
                  const int number_of_iterations,
                  py::object vertex_is_constrained_map,
                  py::object constrained_vertices) {
  if (!CGAL::is_triangle_mesh(mesh))
    throw std::runtime_error("Only triangle meshes can be smoothed!");

  const FaceRange face_range =
      face_group.is_none() ? faces(mesh) : face_group.cast<const FaceRange&>();

  auto [vertex_constraints_option, vertex_constraints] =
      utils::vertex_is_constrained_map_parameter(
          mesh, vertex_is_constrained_map, constrained_vertices);

  namespace pns = CGAL::Polygon_mesh_processing::parameters;

  // WARNING: the underlying parameter type of BGL Named_parameters changes with
  // each
  //          modification so that all_parameters is actually a variant
  auto all_parameters =
      utils::concatenate(pns::number_of_iterations(number_of_iterations),
                         vertex_constraints_option);

  std::visit(
      [&](auto&& np) {
        CGAL::Polygon_mesh_processing::smooth_shape(face_range, mesh, time, np);
      },
      all_parameters);

  if (!constrained_vertices.is_none()) {
    assert(vertex_constraints);
    mesh.remove_property_map(*vertex_constraints);
  }
}
}  // namespace implementation

namespace detail = pyCGAL::wrap::Polygon_mesh_processing::detail;

template <typename PolygonMesh, typename FaceRange>
void wrap_element(detail::smooth_shape<PolygonMesh, FaceRange>,
                  py::module& module) {
  module.def("smooth_shape",
             &implementation::smooth_shape<PolygonMesh, FaceRange>,
             py::arg("mesh").none(false), py::arg("time").none(false),
             py::arg("faces") = py::none(), py::kw_only(),
             py::arg("number_of_iterations") = static_cast<unsigned int>(1),
             py::arg("vertex_is_constrained_map") = py::none(),
             py::arg("constrained_vertices") = py::none());
}

}  // namespace pyCGAL
