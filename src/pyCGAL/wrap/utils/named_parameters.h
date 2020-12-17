#pragma once

#include <CGAL/boost/graph/Named_function_parameters.h>

#include <cassert>
#include <optional>
#include <tuple>
#include <variant>

namespace pyCGAL::wrap::utils {

template <typename T, typename Tag>
using Standalone_parameter =
    CGAL::Named_function_parameters<T, Tag, CGAL::internal_np::No_property>;

template <typename Base, typename P>
struct Parameter_pack;

template <typename Base, typename T, typename Tag>
struct Parameter_pack<Base, Standalone_parameter<T, Tag>> {
  using type = CGAL::Named_function_parameters<T, Tag, Base>;
};

template <typename P0, typename P1>
using parameter_pack_t = typename Parameter_pack<P0, P1>::type;

template <typename Add, typename T>
auto create_optional_parameter(Add add, const std::optional<T>& option) {
  using parameter_type = std::invoke_result_t<Add, T>;
  using Result = std::optional<parameter_type>;
  if (option) {
    const T& t = *option;
    return Result{std::invoke(add, t)};
  }
  return Result{};
}

template <typename... Bases>
auto _concatenate_with_base(const std::variant<Bases...>& vbase) {
  return vbase;
}

template <typename... Bases, typename P, typename... Ps>
auto _concatenate(const std::variant<Bases...>& vbase,
                  const std::optional<P>& option,
                  const std::optional<Ps>&... more_options) {
  auto vpack = std::visit(
      [&](auto&& base) {
        using Result = std::variant<Bases..., parameter_pack_t<Bases, P>...>;
        using Base = std::decay_t<decltype(base)>;
        if (option) return Result{parameter_pack_t<Base, P>{option->v, base}};
        return Result{base};
      },
      vbase);
  if constexpr (sizeof...(Ps) == 0) {
    return vpack;
  } else {
    return _concatenate(vpack, more_options...);
  }
}

template <typename Base, typename... Ps>
auto concatenate(const Base& base, const std::optional<Ps>&... options) {
  return _concatenate(std::variant<Base>{base}, options...);
}

template <std::size_t k = 0, typename... Options>
auto _find_base(std::tuple<Options...> options) {
  auto all_default = CGAL::parameters::all_default();
  using Base = std::variant<decltype(all_default),
                            typename std::decay_t<Options>::value_type...>;
  if constexpr (k >= sizeof...(Options)) {
    return Base{all_default};
  } else {
    auto& option = std::get<k>(options);
    if (option) {
      auto base = Base{*option};
      option.reset();
      assert(!std::get<k>(options));
      return base;
    }
    return _find_base<k + 1>(options);
  }
}

template <typename... Options>
auto concatenate_options(Options... options) {
  auto base = _find_base(std::forward_as_tuple(
      options...));  // options may be modified to skip the base element
  return _concatenate(base, options...);
}

}  // namespace pyCGAL::wrap::utils
