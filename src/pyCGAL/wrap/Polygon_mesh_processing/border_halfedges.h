#pragma once

#include <CGAL/Polygon_mesh_processing/border.h>
#include <pyCGAL/typedefs.h>

#include <stdexcept>
#include <vector>

#include "detail/border_halfedges.h"

namespace pyCGAL {

namespace detail = pyCGAL::wrap::Polygon_mesh_processing::detail;

template <typename PolygonMesh, typename Halfedge_descriptor,
          typename FaceRange>
void wrap_element(
    detail::border_halfedges<PolygonMesh, Halfedge_descriptor, FaceRange>,
    py::module& module) {
  module.def(
      "border_halfedges",
      [](PolygonMesh& mesh, py::object face_group) {
        const FaceRange& face_range = face_group.is_none()
                                          ? faces(mesh)
                                          : face_group.cast<const FaceRange&>();
        std::vector<Halfedge_descriptor> borders;
        CGAL::Polygon_mesh_processing::border_halfedges(face_range, mesh,
                                                        back_inserter(borders));
        return borders;
      },
      py::arg("mesh").none(false), py::arg("face_group") = py::none());
}

}  // namespace pyCGAL
