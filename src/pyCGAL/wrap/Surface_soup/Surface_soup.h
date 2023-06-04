#pragma once

#include <pyCGAL/typedefs.h>
#include <pyCGAL/wrap/Surface_mesh/Property_capsule.h>
#include <pybind11/numpy.h>

#include <vector>

#include "extensions/Surface_soup.h"
#include "extensions/collect_polylines.h"

namespace pyCGAL {

namespace detail {

template <typename Mesh, typename Constraints>
auto extract_and_append_mesh_with_constraints(
    const py::handle obj, std::vector<std::pair<Mesh, Constraints>>& meshes,
    const bool copy = true) {
  Mesh* M = nullptr;
  Constraints constraints;

  if (py::isinstance<py::tuple>(obj)) {
    py::tuple t = obj.cast<py::tuple>();
    M = t[0].cast<Mesh*>();
    auto pmap = t[1];
    if (py::hasattr(pmap, "property_capsule")) {
      using Property_capsule = pyCGAL::wrap::Surface_mesh::Property_capsule<
          Mesh, typename Mesh::Edge_index>;
      typename Constraints::value_type* p = nullptr;
      try {
        auto capsule = pmap.attr("property_capsule")().cast<Property_capsule>();
        p = capsule.template get_map_if<bool>();
      } catch (py::cast_error) {
      }
      if (!p)
        throw std::runtime_error(
            "Could not convert python object to an actual property "
            "map.");
      constraints = *p;
    }
  } else {
    M = obj.cast<Mesh*>();
  }

  if (copy)
    meshes.emplace_back(*M, constraints);
  else
    meshes.emplace_back(std::move(*M), constraints);
}

template <typename Mesh, typename Constraints>
void extract_and_append_surfaces(
    py::list surfaces, std::vector<std::pair<Mesh, Constraints>>& meshes,
    const bool copy = true) {
  for (auto&& obj : surfaces) {
    extract_and_append_mesh_with_constraints(obj, meshes, copy);
  }
}

}  // namespace detail

namespace ext = extensions;

template <typename EMesh>
typename WrapTraits<ext::Surface_soup::Surface_soup<EMesh>>::py_class
wrap_class(WrapTraits<ext::Surface_soup::Surface_soup<EMesh>> wrap,
           py::module& module) {
  using Soup = ext::Surface_soup::Surface_soup<EMesh>;

  import_dependencies<Soup>();

  auto cls = py::class_<Soup>(module, wrap.name);

  cls.def(py::init([](py::list surfaces, const bool copy) {
            std::vector<typename Soup::Mesh_with_constraints> meshes;
            detail::extract_and_append_surfaces(surfaces, meshes, copy);
            return std::make_unique<Soup>(std::move(meshes));
          }),
          py::arg("surfaces").none(false), py::kw_only(),
          py::arg("copy") = false);
  cls.def(py::init([](py::handle clipper, py::list surfaces, const bool copy) {
            std::vector<typename Soup::Mesh_with_constraints> meshes;
            detail::extract_and_append_mesh_with_constraints(clipper, meshes,
                                                             copy);
            detail::extract_and_append_surfaces(surfaces, meshes, copy);
            return std::make_unique<Soup>(std::move(meshes),
                                          /* clip_with_first = */ true);
          }),
          py::arg("clipper").none(false), py::arg("surfaces") = py::list{},
          py::kw_only(), py::arg("copy") = false);

  cls.def_property_readonly(
      "meshes",
      [](Soup& self) {
        return py::make_iterator(self.meshes_begin(), self.meshes_end());
      },
      py::keep_alive<0, 1>());
  cls.def("collect_polylines", [](Soup& self) {
    return ext::Surface_soup::collect_polylines(self.elements);
  });
  cls.def("as_brep", [](Soup& self) {
    auto&& [vertices, facets, facet_index] = self.as_brep();
    static_assert(sizeof(typename decltype(vertices)::value_type) ==
                  3 * sizeof(double));
    std::size_t vshape[2] = {vertices.size(), 3};
    py::array_t<double, py::array::c_style> py_vertices{vshape};
    std::copy(
        reinterpret_cast<const double*>(vertices.data()),
        reinterpret_cast<const double*>(vertices.data()) + 3 * vertices.size(),
        py_vertices.mutable_data());
    static_assert(sizeof(typename decltype(facets)::value_type) ==
                  3 * sizeof(int));
    std::size_t fshape[2] = {facets.size(), 3};
    py::array_t<int, py::array::c_style> py_facets{fshape};
    std::copy(facets.data()->data(), facets.data()->data() + 3 * facets.size(),
              py_facets.mutable_data());
    static_assert(
        std::is_same_v<typename decltype(facet_index)::value_type, int>);
    std::size_t fishape[1] = {facets.size()};
    py::array_t<int, py::array::c_style> py_indices{fishape};
    std::copy(facet_index.begin(), facet_index.end(),
              py_indices.mutable_data());
    return py::make_tuple(py_vertices, py_facets, py_indices);
  });

  return cls;
}

}  // namespace pyCGAL
