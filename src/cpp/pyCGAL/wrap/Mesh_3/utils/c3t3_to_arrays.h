#pragma once

// Adapted from Mesh_3/include/CGAL/IO/File_medit.h
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>

#include <unordered_map>

#include "C3t3_pmaps.h"

namespace py = pybind11;

namespace pyCGAL {
namespace wrap {
namespace utils {

namespace detail {

// the points can be weighted or exact so we convert them to double
template <typename Point>
inline void dump_point(const Point& P, double*& p) {
  *p = CGAL::to_double(P.x());
  ++p;
  *p = CGAL::to_double(P.y());
  ++p;
  *p = CGAL::to_double(P.z());
  ++p;
}

// template <typename VMap, typename VIndex, typename... VHandles>
// void dump_simplex(VMap& V, VIndex*& p, VHandles... vhs) {
//     (*(p++) = V[vhs], ...);
// }

template <typename VMap, typename VIndex, typename VHandle>
inline void dump_simplex(VMap& V, VIndex*& p, VHandle vh0, VHandle vh1) {
  *p = V[vh0];
  ++p;
  *p = V[vh1];
  ++p;
}

template <typename VMap, typename VIndex, typename VHandle>
inline void dump_simplex(VMap& V, VIndex*& p, VHandle vh0, VHandle vh1,
                         VHandle vh2) {
  *p = V[vh0];
  ++p;
  *p = V[vh1];
  ++p;
  *p = V[vh2];
  ++p;
}

template <typename VMap, typename VIndex, typename VHandle>
inline void dump_simplex(VMap& V, VIndex*& p, VHandle vh0, VHandle vh1,
                         VHandle vh2, VHandle vh3) {
  *p = V[vh0];
  ++p;
  *p = V[vh1];
  ++p;
  *p = V[vh2];
  ++p;
  *p = V[vh3];
  ++p;
}

template <typename Range, typename F>
inline auto collect_to_array(Range&& range, F&& f) {
  // CHECKME: We could use std::ranges::transform here
  using pv_type = std::decay_t<decltype(f(range.begin()))>;
  py::array_t<pv_type, py::array::c_style> properties{
      static_cast<py::ssize_t>(range.size())};
  pv_type* pv = properties.mutable_data();
  for (auto it = range.begin(); it != range.end(); ++it) {
    (*pv) = f(it);
    ++pv;
  }
  return properties;
}

template <typename Range, typename F1, typename F2>
inline auto collect_to_array(Range&& range, F1&& f1, F2&& f2) {
  // CHECKME: We could use std::ranges::transform here
  using pv_type = std::decay_t<decltype(f1(range.begin()))>;
  using pv_type2 = std::decay_t<decltype(f2(range.begin()))>;
  static_assert(std::is_same_v<pv_type, pv_type2>);
  py::array_t<pv_type, py::array::c_style> properties{
      static_cast<py::ssize_t>(2 * range.size())};
  pv_type* pv = properties.mutable_data();
  for (auto it = range.begin(); it != range.end(); ++it) {
    (*pv) = f1(it);
    ++pv;
    (*pv) = f2(it);
    ++pv;
  }
  return properties;
}

template <typename C3t3>
inline auto triangulation_vertices(const C3t3& c3t3) {
  const auto& tr = c3t3.triangulation();
  return CGAL::make_range(tr.finite_vertices_begin(), tr.finite_vertices_end());
}

template <typename C3t3>
inline auto c3t3_corners_indices_to_array(const C3t3& c3t3) {
  // we could use: std::mem_fn here
  return collect_to_array(CGAL::make_range(c3t3.vertices_in_complex_begin(),
                                           c3t3.vertices_in_complex_end()),
                          [&c3t3](auto&& vh) { return c3t3.corner_index(vh); });
}

template <typename C3t3>
inline auto c3t3_curves_indices_to_array(const C3t3& c3t3) {
  // we could use: std::mem_fn here
  return collect_to_array(CGAL::make_range(c3t3.edges_in_complex_begin(),
                                           c3t3.edges_in_complex_end()),
                          [&c3t3](auto&& eh) { return c3t3.curve_index(*eh); });
}

template <typename C3t3, typename... PMaps>
inline auto c3t3_facets_indices_to_array(const C3t3& c3t3, PMaps&... pmaps) {
  return collect_to_array(

      CGAL::make_range(c3t3.facets_in_complex_begin(),
                       c3t3.facets_in_complex_end()),
      [&pmaps](auto&& fit) { return get(pmaps, *fit); }...);
}

template <typename C3t3, typename PMap>
inline auto c3t3_cells_indices_to_array(const C3t3& c3t3, PMap& pmap) {
  return collect_to_array(
      CGAL::make_range(c3t3.cells_in_complex_begin(),
                       c3t3.cells_in_complex_end()),
      [&pmap](auto&& cit) { return pmap.subdomain_index(cit); });
}

}  // namespace detail

template <typename C3t3, bool rebind = false, bool no_patch = false,
          typename vindex_type = int>
auto c3t3_to_arrays(const C3t3& c3t3, const bool return_corner_index,
                    const bool return_curve_index,
                    const bool return_facet_index,
                    const bool return_subdomain_index) {
  using Triangulation = typename C3t3::Triangulation;
  using Vertex_handle = typename Triangulation::Vertex_handle;
  using Point = typename Triangulation::Point;  // can be weighted

  // Indices are defined in Domain - check Polyhedral_mesh_domain_*_3.h
  using Surface_patch_index = typename C3t3::Surface_patch_index;
  static_assert(std::is_same_v<Surface_patch_index, int>);

  pyCGAL::utils::C3t3_pmaps<C3t3, rebind, no_patch> pmaps{c3t3};
  const Triangulation& tr = c3t3.triangulation();

  boost::unordered_map<Vertex_handle, int> V;
  std::array<std::size_t, 2> shape{tr.number_of_vertices(),
                                   CGAL::Ambient_dimension<Point>::value};
  py::array_t<double, py::array::c_style> vertices{shape};
  vindex_type i = 0;
  double* p = vertices.mutable_data();
  for (auto vit = tr.finite_vertices_begin(); vit != tr.finite_vertices_end();
       ++vit, ++i) {
    V[vit] = i;
    detail::dump_point(tr.point(vit), p);
  }

  assert(c3t3.number_of_corners() >= 0);
  // FIXME: not available in <utility>
  // assert(std::cmp_less(c3t3.number_of_corners(),
  //                     std::numeric_limits<py::ssize_t>::max()));
  py::array_t<vindex_type, py::array::c_style> corners{
      static_cast<py::ssize_t>(c3t3.number_of_corners())};
  vindex_type* pv = corners.mutable_data();
  int* pi = nullptr;
  for (auto vit = c3t3.vertices_in_complex_begin();
       vit != c3t3.vertices_in_complex_end(); ++vit) {
    (*pv) = V[vit];
    ++pv;
  }

  shape = {c3t3.number_of_edges(), 2};
  py::array_t<vindex_type, py::array::c_style> edges{shape};
  pv = edges.mutable_data();
  for (auto eit = c3t3.edges_in_complex_begin();
       eit != c3t3.edges_in_complex_end(); ++eit) {
    detail::dump_simplex(V, pv, eit->first->vertex(eit->second),
                         eit->first->vertex(eit->third));
  }

  shape = {c3t3.number_of_facets_in_complex() * (pmaps.has_facet_twice ? 2 : 1),
           3};
  py::array_t<vindex_type, py::array::c_style> triangles{shape};
  pv = triangles.mutable_data();
  for (auto fit = c3t3.facets_in_complex_begin();
       fit != c3t3.facets_in_complex_end(); ++fit) {
    auto f = (*fit);
    // cf. comments in CGAL:Mesh_3/include/CGAL/IO/File_medit.h
    if (pmaps.has_facet_twice) {
      if (f.first->subdomain_index() >
          f.first->neighbor(f.second)->subdomain_index())
        f = tr.mirror_facet(f);
    }
    auto vh1 = f.first->vertex((f.second + 1) % 4);
    auto vh2 = f.first->vertex((f.second + 2) % 4);
    auto vh3 = f.first->vertex((f.second + 3) % 4);
    if (f.second % 2 != 0) std::swap(vh2, vh3);
    detail::dump_simplex(V, pv, vh1, vh2, vh3);
    if (pmaps.has_facet_twice) {
      detail::dump_simplex(V, pv, vh3, vh2, vh1);
    }
  }

  shape = {c3t3.number_of_cells_in_complex(), 4};
  py::array_t<vindex_type, py::array::c_style> tets{shape};
  pv = tets.mutable_data();
  for (auto cit = c3t3.cells_in_complex_begin();
       cit != c3t3.cells_in_complex_end(); ++cit) {
    detail::dump_simplex(V, pv, cit->vertex(0), cit->vertex(1), cit->vertex(2),
                         cit->vertex(3));
  }

  py::list result{};
  result.append(vertices);
  result.append(corners);
  result.append(edges);
  result.append(triangles);
  result.append(tets);

  // Indices are defined in Domain - check Polyhedral_mesh_domain_*_3.h
  if (return_corner_index)
    result.append(detail::c3t3_corners_indices_to_array(c3t3));
  if (return_curve_index)
    result.append(detail::c3t3_curves_indices_to_array(c3t3));
  static_assert(std::is_same_v<Surface_patch_index, int>);
  if (return_facet_index)
    if (pmaps.has_facet_twice)
      result.append(detail::c3t3_facets_indices_to_array(
          c3t3, pmaps.facet_pmap, pmaps.facet_pmap_twice));
    else
      result.append(
          detail::c3t3_facets_indices_to_array(c3t3, pmaps.facet_pmap));
  if (return_subdomain_index)
    result.append(detail::c3t3_cells_indices_to_array(c3t3, pmaps.cell_pmap));

  return result;
}

template <typename C3t3, typename index_type = int>
auto collect_all_corner_indices(const C3t3& c3t3) {
  return detail::collect_to_array(
      detail::triangulation_vertices(c3t3), [&c3t3](auto v) -> index_type {
        return static_cast<index_type>(c3t3.corner_index(v));
      });
}

// template <typename C3t3, typename index_type = int>
// auto collect_vertices_index(const C3t3& c3t3) {
//   return detail::collect_to_array(
//       detail::triangulation_vertices(c3t3), [&c3t3](auto v) -> index_type {
//         const auto i = c3t3.index(v);
//         switch (c3t3.in_dimension(v)) {
//         //case 0:
//         //    return c3t3.corner_index(i);
//           case 1:
//             return c3t3.curve_index(i);
//         }
//         assert(false);
//         return index_type{};
//       });
// }

template <typename C3t3, typename dimension_type = int>
auto collect_vertices_dimension(const C3t3& c3t3) {
  return detail::collect_to_array(
      detail::triangulation_vertices(c3t3), [&c3t3](auto v) -> dimension_type {
        return static_cast<dimension_type>(c3t3.in_dimension(v));
      });
}

template <typename C3t3, typename dimension_type = int>
auto collect_tets_with_incident_facets(const C3t3& c3t3) {
  using Triangulation = typename C3t3::Triangulation;
  using Facet = typename Triangulation::Facet;
  using Surface_patch_index = typename C3t3::Surface_patch_index;
  const Triangulation& tr = c3t3.triangulation();
  std::set<Surface_patch_index> incident_patches;
  std::set<Facet> incident_facets;
  py::list result;
  std::size_t k = 0;
  for (auto cit = c3t3.cells_in_complex_begin();
       cit != c3t3.cells_in_complex_end(); ++cit, ++k) {
    if (c3t3.has_incident_facets_in_complex(cit->vertex(0)) ||
        c3t3.has_incident_facets_in_complex(cit->vertex(1)) ||
        c3t3.has_incident_facets_in_complex(cit->vertex(2)) ||
        c3t3.has_incident_facets_in_complex(cit->vertex(3))) {
      incident_patches.clear();
      incident_facets.clear();
      // CHECKME: this could be optimized with a filtered insertion
      for (int i = 0; i < 4; ++i) {
        tr.finite_incident_facets(
            cit->vertex(i),
            std::inserter(incident_facets, incident_facets.end()));
      }
      for (auto&& f : incident_facets) {
        if (c3t3.is_in_complex(f)) {
          const auto i = c3t3.surface_patch_index(f);
          if (i != 2) incident_patches.insert(i);
        }
      }
      if (!incident_patches.empty()) {
        py::list patch_indices;
        for (auto&& i : incident_patches) patch_indices.append(i);
        result.append(py::make_tuple(k, patch_indices));
      }
    }
  }
  return result;
}

}  // namespace utils
}  // namespace wrap
}  // namespace pyCGAL
