#pragma once

#include <vector>

namespace pyCGAL {
namespace addons {
namespace Polygon_mesh_processing {

template <typename Point_type>
struct Polygon_soup {
  using Point = Point_type;
  using Points = std::vector<Point>;
  using Polygon = std::vector<std::size_t>;
  using Polygons = std::vector<Polygon>;
  Points points;
  Polygons polygons;
};

}  // namespace Polygon_mesh_processing
}  // namespace addons
}  // namespace pyCGAL
