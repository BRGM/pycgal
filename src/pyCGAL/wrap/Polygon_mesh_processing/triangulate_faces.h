#pragma once

#include <CGAL/Polygon_mesh_processing/triangulate_faces.h>
#include <pyCGAL/typedefs.h>
#include <pyCGAL/wrap/utils/container_proxy.h>

#include "detail/triangulate_faces.h"

namespace pyCGAL {

namespace detail = pyCGAL::wrap::Polygon_mesh_processing::detail;
namespace utils = pyCGAL::wrap::utils;

template <typename PolygonMesh>
void wrap_element(detail::triangulate_faces<PolygonMesh>, py::module& module) {
  module.def(
      "triangulate_faces",
      [](PolygonMesh& mesh, py::object face_group) {
        if (face_group.is_none()) {
          CGAL::Polygon_mesh_processing::triangulate_faces(mesh);
        } else {
          using Face_index = typename PolygonMesh::Face_index;
          auto group = utils::container_proxy<Face_index>(face_group);
          CGAL::Polygon_mesh_processing::triangulate_faces(group.as_range(),
                                                           mesh);
        }
      },
      py::arg("mesh").none(false), py::arg("face_group") = py::none());
}

}  // namespace pyCGAL
