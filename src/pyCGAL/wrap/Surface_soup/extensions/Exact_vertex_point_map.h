#pragma once

#include <CGAL/Cartesian_converter.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

#include <cassert>

namespace pyCGAL {
namespace extensions {
namespace Surface_soup {
namespace detail {

template <typename Mesh, typename Input_kernel = CGAL::Epick>
struct Exact_vertex_point_map {
  using K = Input_kernel;
  using EK = CGAL::Epeck;
  using Point_3 = typename K::Point_3;
  using Exact_point_3 = typename EK::Point_3;
  using Vertex_index = typename Mesh::Vertex_index;
  using Exact_point_map =
      typename Mesh::template Property_map<Vertex_index, Exact_point_3>;

  // typedef for the property map
  using value_type =
      typename boost::property_traits<Exact_point_map>::value_type;
  using reference = typename boost::property_traits<Exact_point_map>::reference;
  using key_type = typename boost::property_traits<Exact_point_map>::key_type;
  using category = typename boost::read_write_property_map_tag;

  // exterior references
  Exact_point_map exact_point_map;
  Mesh *tm_ptr;  // this must not be invalidated

  // Converters
  CGAL::Cartesian_converter<K, EK> to_exact;
  CGAL::Cartesian_converter<EK, K> to_input;

  Exact_vertex_point_map() : tm_ptr{nullptr} {}

  Exact_vertex_point_map(Mesh &tm, const std::string &name = {}) : tm_ptr{&tm} {
    auto res = tm.template add_property_map<Vertex_index, Exact_point_3>(name);
    assert(res.second);
    exact_point_map = res.first;
    for (auto &&v : vertices(tm)) {
      exact_point_map[v] = to_exact(tm.point(v));
    }
  }

  friend reference get(const Exact_vertex_point_map &map, key_type k) {
    CGAL_precondition(map.tm_ptr != nullptr);
    return map.exact_point_map[k];
  }

  friend void put(const Exact_vertex_point_map &map, key_type k,
                  const EK::Point_3 &p) {
    CGAL_precondition(map.tm_ptr != nullptr);
    map.exact_point_map[k] = p;
    // create the input point from the exact one
    map.tm_ptr->point(k) = map.to_input(p);
  }
};

}  // namespace detail
}  // namespace Surface_soup
}  // namespace extensions
}  // namespace pyCGAL
