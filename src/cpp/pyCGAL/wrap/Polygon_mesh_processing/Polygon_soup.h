#pragma once

#include <pyCGAL/addons/Polygon_mesh_processing/Polygon_soup.h>
#include <pyCGAL/typedefs.h>
#include <pyCGAL/wrap/utils/Vector.h>
#include <pyCGAL/wrap/utils/vector_to_ostream.h>

namespace pyCGAL {

namespace detail {

template <typename Polygons>
void extend_polygons(Polygons& polygons, const py::list& l) {
  using Polygon = typename Polygons::value_type;
  polygons.reserve(polygons.size() + py::len(l));
  for (auto&& x : l) {
    try {
      auto polygon = x.cast<py::list>();
      polygons.emplace_back();
      extend_vector(polygons.back(), polygon);
    } catch (const py::cast_error&) {
      polygons.emplace_back(x.cast<const Polygon&>());
    }
  }
}

}  // namespace detail

template <typename Point>
typename WrapTraits<
    addons::Polygon_mesh_processing::Polygon_soup<Point>>::py_class
wrap_class(
    WrapTraits<addons::Polygon_mesh_processing::Polygon_soup<Point>> wrap,
    py::module& module) {
  using Wrap = WrapTraits<addons::Polygon_mesh_processing::Polygon_soup<Point>>;
  using Polygon_soup = typename Wrap::cpp_type;
  using Polygons = typename Polygon_soup::Polygons;

  auto pycore = py::module_::import("pycgal.core");
  import_dependencies<Polygon_soup>();

  typename Wrap::py_class pyclass = py::class_<Polygon_soup>(module, wrap.name);

  auto pyPolygons = py::class_<Polygons>(pyclass, "Polygons");

  // we register the function first so that it is the first overload alternative
  pyPolygons.def("extend", &detail::extend_polygons<Polygons>);

  wrap_vector_base(pyPolygons);

  pyPolygons.def("push_back", [](Polygons& self, py::list& l) {
    self.emplace_back();
    detail::extend_vector(self.back(), l);
  });
  pyPolygons.def("__str__", [](Polygons& self) {
    using Polygon = typename Polygons::value_type;
    std::stringstream s;
    s << "Polygons";
    auto representation = [](const Polygon& polygon) {
      std::stringstream s;
      s << "Polygon";
      wrap::utils::vector_to_ostream(polygon, s, "", ", ", "");
      return s.str();
    };
    wrap::utils::vector_to_ostream(self, s, "", ", ", "", representation);
    return s.str();
  });

  pyclass.def(py::init<>());
  pyclass.def_readonly("points", &Polygon_soup::points);
  pyclass.def_readonly("polygons", &Polygon_soup::polygons);

  auto pyPolygon = pycore.attr("Vector_of_size_t");
  pyclass.def_property_readonly(
      "Polygon", [pyPolygon](Polygon_soup& self) { return pyPolygon; });

  return pyclass;
}

}  // namespace pyCGAL
