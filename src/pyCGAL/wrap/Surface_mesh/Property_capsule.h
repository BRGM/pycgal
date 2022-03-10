#pragma once

#include <cstdint>
#include <type_traits>
#include <variant>

namespace pyCGAL::wrap::Surface_mesh {

using pointer_type = std::uintptr_t;

using available_property_types =
    std::tuple<bool, char, int, float, double, pointer_type>;

namespace helper_traits {
template <typename Surface_mesh, typename Index, typename Tuple>
struct Properties;

template <typename Surface_mesh, typename Index, typename... Ts>
struct Properties<Surface_mesh, Index, std::tuple<Ts...>> {
  template <typename T>
  using Property_map = typename Surface_mesh::template Property_map<Index, T>;
  using alternatives = std::variant<Property_map<Ts>...>;
};
}  // namespace helper_traits

template <typename Surface_mesh, typename Index>
struct Property_capsule {
  using variant_type = typename helper_traits::Properties<
      Surface_mesh, Index, available_property_types>::alternatives;
  template <typename T>
  using Property_map = typename Surface_mesh::template Property_map<Index, T>;
  template <typename T>
  const Property_map<T>* get_map_if() const {
    return std::get_if<Property_map<T>>(this->ptr);
  }
  template <typename T>
  Property_map<T>* get_map_if() {
    return std::get_if<Property_map<T>>(this->ptr);
  }
  Property_capsule(variant_type& v) : ptr{&v} {}

 private:
  std::add_pointer_t<variant_type> ptr;
};

}  // namespace pyCGAL::wrap::Surface_mesh
