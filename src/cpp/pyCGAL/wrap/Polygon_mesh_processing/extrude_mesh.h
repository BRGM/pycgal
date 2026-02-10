#pragma once

#include <CGAL/Polygon_mesh_processing/extrude.h>

#include "detail/extrude_mesh.h"

namespace pyCGAL {

namespace detail = pyCGAL::wrap::Polygon_mesh_processing::detail;

namespace {

template <typename Vector>
struct Translation {
  Vector V;
  template <typename Point>
  Point operator()(const Point& P) const {
    return P + V;
  }
};

template <typename Vector>
Translation<Vector> make_translation(const Vector& V) {
  return Translation<Vector>{V};
}

template <typename PolygonMesh, typename Vector>
void extrude_mesh_along_vector(const PolygonMesh& input, PolygonMesh& output,
                               const Vector& V) {
  const auto t = make_translation(V);
  CGAL::Polygon_mesh_processing::extrude_mesh(
      input, output, [](auto&& vin, auto&& vout) {},  // does nothing
      [&input, &output, t](auto&& vin, auto&& vout) {
        output.point(vout) = t(input.point(vin));
      });
}

template <typename PolygonMesh, typename Vector>
auto extrude_mesh_along_vector(const PolygonMesh& input, const Vector& V)
    -> PolygonMesh {
  PolygonMesh result;
  extrude_mesh_along_vector(input, result, V);
  return result;
}

template <typename PolygonMesh, typename Vector>
void extrude_mesh_along_vectors(const PolygonMesh& input, PolygonMesh& output,
                                const Vector& Vbottom, const Vector& Vtop) {
  const auto tbottom = make_translation(Vbottom);
  const auto ttop = make_translation(Vtop);
  CGAL::Polygon_mesh_processing::extrude_mesh(
      input, output,
      [&input, &output, tbottom](auto&& vin, auto&& vout) {
        output.point(vout) = tbottom(input.point(vin));
      },
      [&input, &output, ttop](auto&& vin, auto&& vout) {
        output.point(vout) = ttop(input.point(vin));
      });
}

template <typename PolygonMesh, typename Vector>
auto extrude_mesh_along_vectors(const PolygonMesh& input, const Vector& Vbottom,
                                const Vector& Vtop) -> PolygonMesh {
  PolygonMesh result;
  extrude_mesh_along_vectors(input, result, Vbottom, Vtop);
  return result;
}

template <typename PolygonMesh>
void extrude_mesh_with_functors(const PolygonMesh& input, PolygonMesh& output,
                                py::function& bottom, py::function& top) {
  using Point = typename PolygonMesh::Point;
  CGAL::Polygon_mesh_processing::extrude_mesh(
      input, output,
      [&](auto&& vin, auto&& vout) {
        output.point(vout) = py::cast<Point>(bottom(input.point(vin)));
      },
      [&](auto&& vin, auto&& vout) {
        output.point(vout) = py::cast<Point>(top(input.point(vin)));
      });
}

template <typename PolygonMesh>
auto extrude_mesh_with_functors(const PolygonMesh& input, py::function& bottom,
                                py::function& top) -> PolygonMesh {
  PolygonMesh result;
  extrude_mesh_with_functors(input, result, bottom, top);
  return result;
}
}  // namespace

template <typename PolygonMesh>
void wrap_element(detail::extrude_mesh<PolygonMesh>, py::module& module) {
  using Point = typename PolygonMesh::Point;
  using Vector = decltype(Point{} - Point{});
  module.def("extrude_mesh",
             py::overload_cast<const PolygonMesh&, const Vector&>(
                 &extrude_mesh_along_vector<PolygonMesh, Vector>));
  module.def("extrude_mesh",
             py::overload_cast<const PolygonMesh&, PolygonMesh&, const Vector&>(
                 &extrude_mesh_along_vector<PolygonMesh, Vector>));
  module.def(
      "extrude_mesh",
      py::overload_cast<const PolygonMesh&, const Vector&, const Vector&>(
          &extrude_mesh_along_vectors<PolygonMesh, Vector>));
  module.def("extrude_mesh",
             py::overload_cast<const PolygonMesh&, PolygonMesh&, const Vector&,
                               const Vector&>(
                 &extrude_mesh_along_vectors<PolygonMesh, Vector>));
  module.def(
      "extrude_mesh",
      py::overload_cast<const PolygonMesh&, py::function&, py::function&>(
          &extrude_mesh_with_functors<PolygonMesh>));
  module.def("extrude_mesh", py::overload_cast<const PolygonMesh&, PolygonMesh&,
                                               py::function&, py::function&>(
                                 &extrude_mesh_with_functors<PolygonMesh>));
}
}  // namespace pyCGAL
