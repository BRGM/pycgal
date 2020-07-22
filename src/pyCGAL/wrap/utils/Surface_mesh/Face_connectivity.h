#pragma once

#include <pybind11/numpy.h>

namespace py = pybind11;

namespace pyCGAL {
namespace wrap {
namespace utils {
namespace Surface_mesh {

template <typename Surface_mesh>
using Face_connectivity =
    py::array_t<typename Surface_mesh::size_type,
                py::array::c_style | py::array::forcecast>;

}  // namespace Surface_mesh
}  // namespace utils
}  // namespace wrap
}  // namespace pyCGAL
