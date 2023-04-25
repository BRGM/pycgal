#pragma once

#include <pyCGAL/typedefs.h>
#include <pyCGAL/wrap/Surface_mesh/Property_capsule.h>

#include <vector>

#include "extensions/Surface_soup.h"
#include "extensions/collect_polylines.h"

namespace pyCGAL {

namespace ext = extensions;

template <typename EMesh>
typename WrapTraits<ext::Surface_soup::Surface_soup<EMesh>>::py_class
wrap_class(WrapTraits<ext::Surface_soup::Surface_soup<EMesh>> wrap,
           py::module& module) {
  using Soup = ext::Surface_soup::Surface_soup<EMesh>;

  import_dependencies<Soup>();

  auto cls = py::class_<Soup>(module, wrap.name);

  cls.def(
      py::init([](py::list surfaces, const bool copy,
                  const bool use_first_as_clipper) {
        using Mesh = typename EMesh::Mesh;
        using Constraints = typename Soup::Constraints_map;
        std::vector<std::pair<Mesh, Constraints>> meshes;
        for (auto&& S : surfaces) {
          Mesh* M = nullptr;
          Constraints constraints;
          if (py::isinstance<py::tuple>(S)) {
            py::tuple t = S.cast<py::tuple>();
            M = t[0].cast<Mesh*>();
            auto pmap = t[1];
            if (py::hasattr(pmap, "property_capsule")) {
              using Property_capsule =
                  pyCGAL::wrap::Surface_mesh::Property_capsule<
                      Mesh, typename Mesh::Edge_index>;
              typename Constraints::value_type* p = nullptr;
              try {
                auto capsule =
                    pmap.attr("property_capsule")().cast<Property_capsule>();
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
            M = S.cast<Mesh*>();
          }
          if (copy)
            meshes.emplace_back(*M, constraints);
          else
            meshes.emplace_back(std::move(*M), constraints);
        }
        return std::make_unique<Soup>(std::move(meshes), use_first_as_clipper);
      }),
      py::arg("surfaces").none(false), py::kw_only(), py::arg("copy") = false,
      py::arg("use_first_as_clipper") = false);
  cls.def_property_readonly(
      "meshes",
      [](Soup& self) {
        return py::make_iterator(self.meshes_begin(), self.meshes_end());
      },
      py::keep_alive<0, 1>());
  cls.def("collect_intersections", [](Soup& self) {
    return ext::Surface_soup::collect_polylines(self.elements);
  });

  return cls;
}

}  // namespace pyCGAL
