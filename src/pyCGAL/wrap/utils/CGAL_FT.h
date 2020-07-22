#pragma once

#include <CGAL/Kernel_traits.h>

namespace pyCGAL {
namespace wrap {
namespace utils {

template <typename T>
using CGAL_FT = typename CGAL::Kernel_traits<T>::Kernel::FT;

}  // namespace utils
}  // namespace wrap
}  // namespace pyCGAL
