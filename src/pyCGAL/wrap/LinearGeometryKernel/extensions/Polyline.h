#pragma once

#include <vector>

namespace pyCGAL {
namespace extensions {
namespace LinearGeometryKernel {

template <typename Point_>
struct Polyline {
  using Point = Point_;
  using container = std::vector<Point>;
  container points;
  auto begin() { return points.begin(); }
  auto end() { return points.end(); }
  auto begin() const { return points.begin(); }
  auto end() const { return points.end(); }
  auto size() const { return points.size(); }
  template <typename Node>
  void add_node(Node&& node) {
    points.emplace_back(std::forward<Node>(node));
  }
};

}  // namespace LinearGeometryKernel
}  // namespace extensions
}  // namespace pyCGAL
