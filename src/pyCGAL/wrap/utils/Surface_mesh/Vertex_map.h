#pragma once

namespace pyCGAL {
namespace wrap {
namespace utils {
namespace Surface_mesh {

template <typename Surface_mesh>
using Vertex_map = std::vector<typename Surface_mesh::Vertex_index>;

}  // namespace Surface_mesh
}  // namespace utils
}  // namespace wrap
}  // namespace pyCGAL
