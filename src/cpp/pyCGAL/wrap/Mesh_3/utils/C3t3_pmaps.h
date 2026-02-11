#pragma once

// Adapted from Mesh_3/include/CGAL/IO/File_medit.h
#include <CGAL/IO/File_medit.h>

namespace pyCGAL {
namespace utils {

template <typename C3T3, bool rebind = false, bool no_patch = false>
struct C3t3_pmaps {
  // FIXME: enums were introduced in CGAL 6
  //        is the conversion bool -> enum correct?
  using Generator = CGAL::SMDS_3::Medit_pmap_generator<
      C3T3,
      rebind ? CGAL::SMDS_3::RENUMBER_SUBDOMAINS
             : CGAL::SMDS_3::USE_SUBDOMAIN_INDICES,
      no_patch ? CGAL::SMDS_3::USE_CELL_INDICES
               : CGAL::SMDS_3::RENUMBER_SURFACE_PATCH_INDICES>;
  typename Generator::Cell_pmap cell_pmap;
  typename Generator::Facet_pmap facet_pmap;
  typename Generator::Facet_pmap_twice facet_pmap_twice;
  typename Generator::Vertex_pmap vertex_pmap;
  bool has_facet_twice;

  C3t3_pmaps(const C3T3& c3t3)
      : cell_pmap{c3t3},
        facet_pmap{c3t3, cell_pmap},
        facet_pmap_twice{c3t3, cell_pmap},
        vertex_pmap{c3t3, cell_pmap, facet_pmap},
        has_facet_twice{Generator().print_twice()} {}
};

}  // namespace utils
}  // namespace pyCGAL
