#pragma once

#include <sstream>

namespace pyCGAL {

template <typename Class>
struct Dependencies {
  static std::string get() { return ""; }
};

}  // namespace pyCGAL
