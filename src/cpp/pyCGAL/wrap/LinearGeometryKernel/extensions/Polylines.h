#pragma once

#include <vector>

namespace pyCGAL {
namespace extensions {
namespace LinearGeometryKernel {

template <typename Polyline_>
struct Polylines {
  using Polyline = Polyline_;
  using container = std::vector<Polyline>;
  struct back_insert_iterator {
    container& polylines;
    template <typename T>
    void operator=(T&& t) {
      polylines.emplace_back(Polyline{std::forward<T>(t)});
    }
    back_insert_iterator& operator*() { return *this; }
    // cf. cppreference: Incrementing the std::back_insert_iterator is a noop.
    back_insert_iterator& operator++() { return *this; }
    back_insert_iterator& operator++(int) { return *this; }
  };
  container polylines;
  auto back_inserter() { return back_insert_iterator{polylines}; }
  auto begin() { return polylines.begin(); }
  auto end() { return polylines.end(); }
  auto begin() const { return polylines.begin(); }
  auto end() const { return polylines.end(); }
  auto size() const { return polylines.size(); }
  void start_new_polyline() { polylines.emplace_back(); }
  template <typename Node>
  void add_node(Node&& node) {
    polylines.back().add_node(std::forward<Node>(node));
  }
};

}  // namespace LinearGeometryKernel
}  // namespace extensions
}  // namespace pyCGAL
