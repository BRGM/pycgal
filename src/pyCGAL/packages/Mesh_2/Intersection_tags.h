#pragma once

#include <CGAL/Constrained_triangulation_2.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

namespace pyCGAL::Mesh_2 {

template <typename Traits>
struct Intersection_tag {};

template <>
struct Intersection_tag<CGAL::Epick> {
  using type = CGAL::Exact_predicates_tag;
};

template <>
struct Intersection_tag<CGAL::Epeck> {
  using type = CGAL::Exact_intersections_tag;
};

}  // namespace pyCGAL::Mesh_2
