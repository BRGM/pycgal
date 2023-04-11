#pragma once

#include <pyCGAL/typedefs.h>

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

  cls.def(py::init([](py::list surfaces, const bool copy) {
            using Mesh = typename EMesh::Mesh;
            std::vector<Mesh> meshes;
            for (auto&& S : surfaces) {
              auto& M = S.cast<Mesh&>();
              if (copy)
                meshes.emplace_back(M);
              else
                meshes.emplace_back(std::move(M));
            }
            if (copy) return std::make_unique<Soup>(meshes);
            return std::make_unique<Soup>(std::move(meshes));
          }),
          py::arg("surfaces").none(false), py::arg("copy") = false);
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
