#pragma once

#include <CGAL/Line_3.h>
#include <CGAL/Ray_3.h>
#include <CGAL/Segment_3.h>
#include <CGAL/boost/graph/helpers.h>
#include <pyCGAL/typedefs.h>
#include <pybind11/stl.h>

// #include <optional>
#include <string>

#include "detail/Tree.h"

// namespace pybind11 {
// namespace detail {
// // Intersection objects are boost::optional
// template <typename T>
// struct type_caster<std::optional<T>> : optional_caster<std::optional<T>> {};
// // Intersection results are boost::variant
// template <typename... Ts>
// struct type_caster<boost::variant<Ts...>>
//     : variant_caster<boost::variant<Ts...>> {};
// // Specifies the function used to visit the variant -- `apply_visitor`
// instead
// // of `visit`
// template <>
// struct visit_helper<boost::variant> {
//   template <typename... Args>
//   static auto call(Args&&... args) -> decltype(boost::apply_visitor(args...))
//   {
//     return boost::apply_visitor(args...);
//   }
// };
// }  // namespace detail
// }  // namespace pybind11

namespace pyCGAL {

namespace SM_detail = wrap::Surface_mesh::detail;

template <typename Tree, typename Query, typename pyTree>
void wrap_intersection_predicates(pyTree& pyclass) {
  pyclass.def("any_intersection",
              py::overload_cast<const Query&>(
                  &Tree::template any_intersection<Query>, py::const_));
  pyclass.def("all_intersections", [](const Tree& self, const Query& query) {
    using Intersection =
        typename Tree::template Intersection_and_primitive_id<Query>::Type;
    std::vector<Intersection> intersections;
    self.all_intersections(query, back_inserter(intersections));
    return intersections;
  });
}

template <typename Surface_mesh>
using CGALTree = typename SM_detail::Tree<Surface_mesh>::type;

template <typename Surface_mesh>
py::class_<CGALTree<Surface_mesh>> wrap_class(
    WrapTraits<SM_detail::Tree<Surface_mesh>>, py::module& module) {
  using Wrap = WrapTraits<SM_detail::Tree<Surface_mesh>>;
  using Tree = typename Wrap::cpp_type;
  using CGAL_tree = CGALTree<Surface_mesh>;
  using Kernel = typename Tree::Kernel;
  using Line = CGAL::Line_3<Kernel>;
  using Segment = CGAL::Segment_3<Kernel>;
  using Ray = CGAL::Ray_3<Kernel>;

  auto pyclass = py::class_<CGAL_tree>(module, "Tree");
  pyclass.def(py::init([](const Surface_mesh& mesh) {
    if (!CGAL::is_triangle_mesh(mesh))
      throw std::runtime_error("Trees must be built upon triangle meshes.");
    return std::make_unique<CGAL_tree>(mesh.faces().begin(), mesh.faces().end(),
                                       mesh);
  }));

  pyclass.def("bbox", &CGAL_tree::bbox);
  pyclass.def(
      "first_intersected_primitive",
      py::overload_cast<const Ray&>(
          &CGAL_tree::template first_intersected_primitive<Ray>, py::const_));
  pyclass.def("first_intersection",
              py::overload_cast<const Ray&>(
                  &CGAL_tree::template first_intersection<Ray>, py::const_));

  wrap_intersection_predicates<CGAL_tree, Line>(pyclass);
  wrap_intersection_predicates<CGAL_tree, Ray>(pyclass);
  wrap_intersection_predicates<CGAL_tree, Segment>(pyclass);

  return pyclass;
}

}  // namespace pyCGAL
