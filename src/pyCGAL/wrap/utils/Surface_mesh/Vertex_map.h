#pragma once

namespace pyCGAL {
namespace wrap {
namespace utils {
namespace SM {

template <typename Surface_mesh>
using Vertex_map = std::vector<typename Surface_mesh::Vertex_index>;

}  // namespace SM
}  // namespace utils
}  // namespace wrap
}  // namespace pyCGAL
