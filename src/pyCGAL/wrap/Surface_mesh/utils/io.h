#pragma once

#include <CGAL/Surface_mesh.h>

#include <fstream>

namespace pyCGAL::wrap::utils {

template <typename P>
bool read_off(CGAL::Surface_mesh<P>& sm, const std::string& filename) {
  std::ifstream is{filename};
  return read_off(is, sm);
}

template <typename P>
bool write_off(const CGAL::Surface_mesh<P>& sm, const std::string& filename) {
  std::ofstream os{filename};
  return write_off(os, sm);
}

}  // namespace pyCGAL::wrap::utils
