#pragma once

#include <CGAL/Surface_mesh/IO/OFF.h>

#include <fstream>

namespace pyCGAL::wrap::utils {

template <typename P>
bool read_off(CGAL::Surface_mesh<P>& sm, const std::string& filename) {
  std::ifstream is{filename};
  return CGAL::IO::read_OFF(is, sm);
}

template <typename P>
bool write_off(const CGAL::Surface_mesh<P>& sm, const std::string& filename) {
  std::ofstream os{filename};
  return CGAL::IO::write_OFF(os, sm);
}

}  // namespace pyCGAL::wrap::utils
