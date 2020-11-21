#pragma once

#include <CGAL/boost/graph/Named_function_parameters.h>

#include <optional>
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

}  // namespace pyCGAL::wrap::utils
