#pragma once

namespace pyCGAL::wrap::utils {

template <typename Surface_mesh>
using Vertex_map = std::vector<typename Surface_mesh::Vertex_index>;

}  // namespace pyCGAL::wrap::utils
