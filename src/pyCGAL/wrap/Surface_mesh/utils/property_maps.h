#pragma once

#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>

#include <optional>
#include <tuple>
#include <variant>

namespace py = pybind11;

#include <pyCGAL/wrap/utils/Coordinates_array.h>

#include "extend_mesh.h"

namespace pyCGAL::wrap::utils {

namespace detail {

// we rely on typeid but must some name are very mangled
// and we prefer to keep things under control with tempalte specialization when
// necessary...
template <typename T>
struct TName {
  static constexpr const char* name() { return typeid(T).name(); }
};

template <>
struct TName<bool> {
  static constexpr const char* name() { return "bool"; }
};

template <>
struct TName<int> {
  static constexpr const char* name() { return "int"; }
};

template <>
struct TName<float> {
  static constexpr const char* name() { return "float"; }
};

template <>
struct TName<double> {
  static constexpr const char* name() { return "double"; }
};

template <typename Surface_mesh, typename Index, typename Tuple>
struct Pmap_holder;

template <typename Surface_mesh, typename Index, typename... Ts>
struct Pmap_holder<Surface_mesh, Index, std::tuple<Ts...>> {
  using property_alternatives = std::tuple<Ts...>;
  // static constexpr std::size_t
  //    alternative_size[std::tuple_size_v<property_alternatives>] =
  //    {sizeof(Ts)...};
  template <typename T>
  using Property_map = typename Surface_mesh::template Property_map<Index, T>;
  std::variant<Property_map<Ts>...> map;
  constexpr std::string property_type() const {
    return std::visit(
        [&](auto alternative) { return TName<decltype(alternative)>::name(); },
        map);
  }
  py::object get(const Index i) {
    return std::visit(
        [i](auto alternative) { return py::cast(alternative[i]); }, map);
  }
  void set(const Index i, py::object& value) {
    std::visit(
        [i, &value](auto alternative) {
          using value_type = typename decltype(alternative)::value_type;
          alternative[i] = value.cast<value_type>();
        },
        map);
  }
  struct Buffer_visitor {
    template <typename T>
    py::buffer_info operator()(Property_map<T>& pmap) {
      return py::buffer_info(&(*(pmap.begin())),
                             std::distance(pmap.begin(), pmap.end()));
    }
    py::buffer_info operator()(Property_map<bool>& pmap) {
      return py::buffer_info{};
    }
  };
  py::buffer_info buffer_info() {
    // bool is not handled because it is stored as bit array...
    if (std::holds_alternative<Property_map<bool>>(map))
      throw std::runtime_error(
          "Cannot acces a boolean property map as buffer!");
    return std::visit(Buffer_visitor(), map);
  }
  py::iterator make_iterator() {
    return std::visit(
        [](auto alternative) {
          return py::make_iterator(alternative.begin(), alternative.end());
        },
        map);
  }
  void remove_from_mesh(Surface_mesh& mesh) {
    std::visit(
        [&mesh](auto alternative) { mesh.remove_property_map(alternative); },
        map);
  }
  py::object copy_as_array() {
    return std::visit(
        [](auto alternative) {
          using value_type = typename decltype(alternative)::value_type;
          const std::size_t n =
              std::distance(alternative.begin(), alternative.end());
          py::array_t<value_type, py::array::c_style> res{n};
          std::copy(alternative.begin(), alternative.end(),
                    reinterpret_cast<value_type*>(res.request().ptr));
          return py::object{res};
        },
        map);
  }
};

template <typename SurfaceMesh, typename IndexType>
struct Helper_traits {
  using Surface_mesh = SurfaceMesh;
  using index = IndexType;
  using alternatives = std::tuple<bool, int, float, double>;
  using holder = Pmap_holder<Surface_mesh, index, alternatives>;
};

template <typename Traits, typename Tuple>
struct Find_property;

template <typename Traits>
struct Find_property<Traits, std::tuple<>> {
  using Result = std::optional<typename Traits::holder>;
  static Result check(typename Traits::Surface_mesh&, const std::string&) {
    return Result{};
  }
};

template <typename Traits, typename T, typename... Ts>
struct Find_property<Traits, std::tuple<T, Ts...>> {
  using Result = std::optional<typename Traits::holder>;
  static Result check(typename Traits::Surface_mesh& mesh,
                      const std::string& name) {
    using Index = typename Traits::index;
    auto tmp = mesh.template property_map<Index, T>(name);
    if (tmp.second) return Result{typename Traits::holder{tmp.first}};
    return Find_property<Traits, std::tuple<Ts...>>::check(mesh, name);
  }
};

}  // namespace detail

template <typename Traits, typename T>
py::tuple add_property_map(typename Traits::Surface_mesh& mesh,
                           const std::string& name,
                           const T default_value = T()) {
  using Index = typename Traits::index;
  auto res = mesh.template add_property_map<Index>(name, default_value);
  return py::make_tuple(typename Traits::holder{res.first}, res.second);
}

template <typename Traits>
py::object find_property_holder(typename Traits::Surface_mesh& mesh,
                                const std::string& name) {
  auto res =
      detail::Find_property<Traits, typename Traits::alternatives>::check(mesh,
                                                                          name);
  if (res) return py::cast(*res);
  return py::none();
}

template <typename Index, typename Surface_mesh>
void wrap_property_map(py::module& module, py::class_<Surface_mesh>& pymesh,
                       const std::string& location_name) {
  using traits = detail::Helper_traits<Surface_mesh, Index>;
  using holder = typename traits::holder;

  auto pyholder = py::class_<holder>(
      module, (location_name + "_property").c_str(), py::buffer_protocol());
  pyholder.def("property_type", &holder::property_type);
  pyholder.def("__getitem__", &holder::get);
  pyholder.def("__setitem__", &holder::set);
  pyholder.def_buffer(&holder::buffer_info);
  pyholder.def("__iter__", &holder::make_iterator);
  pyholder.def("copy_as_array", &holder::copy_as_array);

  pymesh.def(("add_" + location_name + "_property").c_str(),
             [](Surface_mesh& mesh, const std::string& name, const char dtype) {
               switch (dtype) {
                 case 'b':
                   return add_property_map<traits, bool>(mesh, name);
                 case 'i':
                   return add_property_map<traits, int>(mesh, name);
                 case 'f':
                   return add_property_map<traits, float>(mesh, name);
                 case 'd':
                   return add_property_map<traits, double>(mesh, name);
                 default:
                   break;
               }
               assert(false);
               return py::make_tuple(py::none(), false);
             },
             py::arg("name").none(false), py::arg("dtype") = 'b');

  pymesh.def((location_name + "_property").c_str(),
             &find_property_holder<traits>);

  pymesh.def((location_name + "_properties").c_str(), [](Surface_mesh& mesh) {
    py::list names;
    for (auto&& name : mesh.template properties<Index>()) names.append(name);
    return names;
  });

  pymesh.def("remove_property", [](Surface_mesh& mesh, holder& pmap) {
    pmap.remove_from_mesh(mesh);
  });
}

}  // namespace pyCGAL::wrap::utils
