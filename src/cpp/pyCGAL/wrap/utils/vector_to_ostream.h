#pragma once

#include <ostream>
#include <vector>

namespace pyCGAL::wrap::utils {

template <typename T, typename Representation_functor>
std::ostream& vector_to_ostream(const std::vector<T>& v, std::ostream& os,
                                const std::string& left_delimiter,
                                const std::string& separator,
                                const std::string& right_delimiter,
                                const Representation_functor& representation) {
  os << "[";
  auto n = v.size();
  if (n <= 4) {
    for (auto&& x : v) {
      os << left_delimiter << representation(x) << right_delimiter;
      --n;
      if (n != 0) os << separator;
    }
  } else {
    os << left_delimiter << representation(v[0]) << right_delimiter
       << separator;
    os << left_delimiter << representation(v[1]) << right_delimiter
       << separator;
    os << "..." << separator;
    os << left_delimiter << representation(v[n - 2]) << right_delimiter
       << separator;
    os << left_delimiter << representation(v[n - 1]) << right_delimiter;
  }
  os << "]";
  return os;
}

template <typename T>
std::ostream& vector_to_ostream(const std::vector<T>& v, std::ostream& os,
                                const std::string& left_delimiter,
                                const std::string& separator,
                                const std::string& right_delimiter) {
  auto representation = [](const T& t) {
    std::stringstream s;
    s << t;
    return s.str();
  };
  return vector_to_ostream(v, os, left_delimiter, separator, right_delimiter,
                           representation);
}

}  // namespace pyCGAL::wrap::utils
