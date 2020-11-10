#pragma once

#include <CGAL/Kernel_traits.h>

namespace pyCGAL::wrap::utils {

template <typename T>
using CGAL_FT = typename CGAL::Kernel_traits<T>::Kernel::FT;

}  // namespace pyCGAL::wrap::utils
