#pragma once

#include <pyCGAL/wrap/LinearGeometryKernel/extensions/Polylines.h>

#include "Polyline_3.h"

namespace pyCGAL::Epick {
using Polylines_3 =
    pyCGAL::extensions::LinearGeometryKernel::Polylines<Polyline_3>;
}  // namespace pyCGAL::Epick
