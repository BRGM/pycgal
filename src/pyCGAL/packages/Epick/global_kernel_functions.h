#pragma once

#include <pyCGAL/wrap/LinearGeometryKernel/detail/global_kernel_functions.h>

#include "Kernel.h"

namespace pyCGAL::Epick {
using global_kernel_functions =
    pyCGAL::wrap::LinearGeometryKernel::detail::global_kernel_functions<Kernel>;
}  // namespace pyCGAL::Epick
