#pragma once

#include <CGAL/Delaunay_mesh_size_criteria_2.h>
#include <CGAL/centroid.h>

namespace pyCGAL {
namespace addons {
template <class CDT>
class Delaunay_mesh_adaptative_size_criteria_2
    : public virtual CGAL::Delaunay_mesh_criteria_2<CDT> {
 protected:
  typedef typename CDT::Geom_traits Geom_traits;
  typedef typename Geom_traits::Point_2 Point_2;
  typedef typename CGAL::Delaunay_mesh_size_criteria_2<CDT>
      Constant_size_criteria;
  typedef std::function<double(const Point_2&)> Sizing_field;
  Sizing_field sizing_field;

 public:
  typedef CGAL::Delaunay_mesh_criteria_2<CDT> Base;

  Delaunay_mesh_adaptative_size_criteria_2()
      : Base{0.125, Geom_traits()}, sizing_field{[](const Point_2& P) {
          return 0;
        }} {}

  Delaunay_mesh_adaptative_size_criteria_2(
      const Sizing_field& field, const double aspect_bound = 0.125,
      const Geom_traits& traits = Geom_traits())
      : Base{aspect_bound, traits}, sizing_field{field} {}

  Delaunay_mesh_adaptative_size_criteria_2(
      const Delaunay_mesh_adaptative_size_criteria_2&) = default;

  struct Quality : Constant_size_criteria::Quality {
    typedef typename Constant_size_criteria::Quality Quality_base;
    using Quality_base::Quality_base;
  };

  class Is_bad : public CGAL::Delaunay_mesh_criteria_2<CDT>::Is_bad {
   protected:
    Sizing_field sizing_field;

   public:
    typedef typename CGAL::Delaunay_mesh_criteria_2<CDT>::Is_bad Is_bad_base;

    Is_bad(const double& b, const Sizing_field& sf, const Geom_traits& gt)
        : Is_bad_base{b, gt}, sizing_field{sf} {}

    auto _local_is_bad(const Point_2& P) const {
      const Constant_size_criteria local_criteria{this->B, sizing_field(P),
                                                  this->traits};
      return local_criteria.is_bad_object();
    }

    CGAL::Mesh_2::Face_badness operator()(const Quality q) const {
      if (q.size() > 1) return CGAL::Mesh_2::IMPERATIVELY_BAD;
      if (q.sine() < this->B)
        return CGAL::Mesh_2::BAD;
      else
        return CGAL::Mesh_2::NOT_BAD;
    }

    CGAL::Mesh_2::Face_badness operator()(const typename CDT::Face_handle& fh,
                                          Quality& q) const {
      const Point_2& pa = fh->vertex(0)->point();
      const Point_2& pb = fh->vertex(1)->point();
      const Point_2& pc = fh->vertex(2)->point();
      const auto is_bad = _local_is_bad(CGAL::centroid(pa, pb, pc));
      return is_bad(fh, q);
    }
  };

  Is_bad is_bad_object() const {
    return Is_bad{this->bound(), sizing_field, this->traits};
  }
};

}  // namespace addons
}  // namespace pyCGAL
