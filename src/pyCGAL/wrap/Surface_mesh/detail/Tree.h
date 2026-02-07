#pragma once

#include <CGAL/AABB_face_graph_triangle_primitive.h>
#include <CGAL/AABB_traits_2.h>
#include <CGAL/AABB_traits_3.h>
#include <CGAL/AABB_tree.h>
#include <CGAL/Kernel_traits.h>

namespace pyCGAL::wrap::Surface_mesh::detail {

template <typename Surface_mesh>
struct Tree {
  using Point = typename Surface_mesh::Point;
  using Kernel = typename CGAL::Kernel_traits<Point>::Kernel;
  using Primitive = CGAL::AABB_face_graph_triangle_primitive<Surface_mesh>;
  using Traits = std::conditional_t<CGAL::Ambient_dimension<Point>::value == 2,
                                    CGAL::AABB_traits_2<Kernel, Primitive>,
                                    CGAL::AABB_traits_3<Kernel, Primitive>>;
  using type = CGAL::AABB_tree<Traits>;
};

}  // namespace pyCGAL::wrap::Surface_mesh::detail
