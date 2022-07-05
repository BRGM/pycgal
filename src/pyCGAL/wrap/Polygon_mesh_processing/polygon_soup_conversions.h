#pragma once

#include <CGAL/Polygon_mesh_processing/polygon_mesh_to_polygon_soup.h>
#include <CGAL/Polygon_mesh_processing/polygon_soup_to_polygon_mesh.h>
#include <pyCGAL/typedefs.h>

#include "detail/polygon_soup_conversions.h"

namespace pyCGAL {

namespace detail = pyCGAL::wrap::Polygon_mesh_processing::detail;

template <typename PolygonSoup, typename PolygonMesh>
void wrap_element(detail::polygon_soup_conversions<PolygonSoup, PolygonMesh>,
                  py::module& module) {
  import_dependencies<
      detail::polygon_soup_conversions<PolygonSoup, PolygonMesh>>();

  module.def("polygon_soup_to_polygon_mesh",
             [](const PolygonSoup& soup, PolygonMesh& mesh) {
               CGAL::Polygon_mesh_processing::polygon_soup_to_polygon_mesh(
                   soup.points, soup.polygons, mesh);
             });
  module.def("polygon_soup_to_polygon_mesh", [](const PolygonSoup& soup) {
    auto mesh = std::make_unique<PolygonMesh>();
    CGAL::Polygon_mesh_processing::polygon_soup_to_polygon_mesh(
        soup.points, soup.polygons, *mesh);
    return mesh;
  });
  module.def("polygon_mesh_to_polygon_soup",
             [](const PolygonMesh& mesh, PolygonSoup& soup) {
               CGAL::Polygon_mesh_processing::polygon_mesh_to_polygon_soup(
                   mesh, soup.points, soup.polygons);
             });
  module.def("polygon_mesh_to_polygon_soup", [](const PolygonMesh& mesh) {
    auto soup = std::make_unique<PolygonSoup>();
    CGAL::Polygon_mesh_processing::polygon_mesh_to_polygon_soup(
        mesh, soup->points, soup->polygons);
    return soup;
  });
}

}  // namespace pyCGAL
