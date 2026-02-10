#pragma once

namespace pyCGAL::wrap::utils {

template <typename Surface_mesh>
using Face_map = std::vector<typename Surface_mesh::Face_index>;

}  // namespace pyCGAL::wrap::utils
