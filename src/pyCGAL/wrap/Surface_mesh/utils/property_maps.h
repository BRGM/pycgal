#pragma once

#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>

#include <optional>
#include <tuple>
#include <variant>

namespace py = pybind11;

#include <pyCGAL/wrap/Surface_mesh/Property_capsule.h>
#include <pyCGAL/wrap/utils/Coordinates_array.h>

#include "selection.h"

namespace pyCGAL::wrap::utils {

using pointer_type = Surface_mesh::pointer_type;

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
struct TName<char> {
  static constexpr const char* name() { return "char"; }
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

template <>
struct TName<pointer_type> {
  static constexpr const char* name() { return "pointer"; }
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
  using Property_index = Index;
  std::variant<Property_map<Ts>...> map;
  template <typename T>
  const Property_map<T>* get_underlying_map() const {
    return std::get_if<Property_map<T>>(&map);
  }
  template <typename T>
  Property_map<T>* get_underlying_map() {
    return std::get_if<Property_map<T>>(&map);
  }
  constexpr std::string property_type() const {
    return std::visit(
        [&](auto alternative) { return TName<decltype(alternative)>::name(); },
        map);
  }
  py::object get(const Index i) {
    return std::visit(
        [i](auto alternative) { return py::cast(alternative[i]); }, map);
  }
  bool is_set(const Index i) const {
    return std::visit(
        [i](auto alternative) {
          if constexpr (std::is_same_v<
                            typename decltype(alternative)::value_type, bool>) {
            return static_cast<bool>(alternative[i]);
          } else {
            throw std::runtime_error(
                "Property map does not hold flags (i.e. boolean types).");
            return false;
          }
        },
        map);
  }
  void set(const Index i, py::object& value) {
    std::visit(
        [i, &value](auto alternative) {
          using value_type = typename decltype(alternative)::value_type;
          alternative[i] = value.cast<value_type>();
        },
        map);
  }
  void set(const std::vector<Index>& indices, py::iterable& values) {
    std::visit(
        [&indices, &values](auto alternative) {
          using value_type = typename decltype(alternative)::value_type;
          auto pi = cbegin(indices);
          for (auto&& value : values) {
            assert(pi != cend(indices));
            alternative[*pi] = value.cast<value_type>();
            ++pi;
          }
        },
        map);
  }
  void set(const std::vector<Index>& indices, py::object& value) {
    std::visit(
        [&indices, &value](auto alternative) {
          using value_type = typename decltype(alternative)::value_type;
          const auto v = value.cast<value_type>();
          for (auto&& i : indices) {
            alternative[i] = v;
          }
        },
        map);
  }
  void fill(py::object& value) {
    std::visit(
        [&value](auto alternative) {
          using value_type = typename decltype(alternative)::value_type;
          const auto v = value.cast<value_type>();
          std::fill(alternative.begin(), alternative.end(), v);
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
          const py::ssize_t n =
              std::distance(alternative.begin(), alternative.end());
          py::array_t<value_type, py::array::c_style> res{n};
          std::copy(alternative.begin(), alternative.end(),
                    reinterpret_cast<value_type*>(res.request().ptr));
          return py::object{res};
        },
        map);
  }
};

template <typename Tuple>
struct Add;

template <typename T, typename... Ts>
struct Add<std::tuple<T, Ts...>> {
  template <typename Holder, typename Surface_mesh>
  static void select(py::class_<Surface_mesh>& pymesh) {
    using Index = typename Holder::Property_index;
    pymesh.def("select",
               [](const Surface_mesh& mesh, const Holder& pmap, T value) {
                 auto p = pmap.template get_underlying_map<T>();
                 assert(p);
                 return select_indices<Surface_mesh, Index, T>(mesh, *p, value);
               });
    if constexpr (sizeof...(Ts) > 0) {
      Add<std::tuple<Ts...>>::template select<Holder>(pymesh);
    }
  }
};

template <typename Holder, typename Surface_mesh>
void add_select(py::class_<Surface_mesh>& pymesh) {
  using alternatives = typename Holder::property_alternatives;
  Add<alternatives>::template select<Holder>(pymesh);
}

template <typename SurfaceMesh, typename IndexType>
struct Helper_traits {
  using Surface_mesh = SurfaceMesh;
  using index = IndexType;
  using alternatives = pyCGAL::wrap::Surface_mesh::available_property_types;
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
                           const std::string& name, py::object& default_value) {
  using Index = typename Traits::index;
  const auto value = default_value.is_none() ? T() : default_value.cast<T>();
  auto res = mesh.template add_property_map<Index>(name, value);
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

  using Property_capsule =
      pyCGAL::wrap::Surface_mesh::Property_capsule<Surface_mesh, Index>;

  py::class_<Property_capsule>(module,
                               (location_name + "_property_capsule").c_str());

  auto pyholder = py::class_<holder>(
      module, (location_name + "_property").c_str(), py::buffer_protocol());
  pyholder.def(
      "property_capsule",
      [](holder& self) { return std::make_unique<Property_capsule>(self.map); },
      py::keep_alive<0, 1>());
  pyholder.def("property_type", &holder::property_type);
  pyholder.def("__getitem__", &holder::get);
  pyholder.def("is_set", &holder::is_set);
  pyholder.def("__setitem__",
               py::overload_cast<Index, py::object&>(&holder::set));
  pyholder.def("set", py::overload_cast<Index, py::object&>(&holder::set));
  pyholder.def("set",
               py::overload_cast<const std::vector<Index>&, py::iterable&>(
                   &holder::set));
  pyholder.def("set", py::overload_cast<const std::vector<Index>&, py::object&>(
                          &holder::set));
  pyholder.def("set", &holder::fill);
  pyholder.def("fill", &holder::fill);
  pyholder.def_buffer(&holder::buffer_info);
  pyholder.def("__iter__", &holder::make_iterator);
  pyholder.def("copy_as_array", &holder::copy_as_array);

  pymesh.def(("add_" + location_name + "_property").c_str(),
             [](Surface_mesh& mesh, const std::string& name, const char dtype,
                py::object value) {
               switch (dtype) {
                 case 'b':
                   return add_property_map<traits, bool>(mesh, name, value);
                 case 'c':
                   return add_property_map<traits, char>(mesh, name, value);
                 case 'i':
                   return add_property_map<traits, int>(mesh, name, value);
                 case 'f':
                   return add_property_map<traits, float>(mesh, name, value);
                 case 'd':
                   return add_property_map<traits, double>(mesh, name, value);
                 case 'p':
                   return add_property_map<traits, pointer_type>(mesh, name,
                                                                 value);
                 default:
                   break;
               }
               assert(false);
               return py::make_tuple(py::none(), false);
             },
             py::arg("name").none(false), py::arg("dtype") = 'b',
             py::arg("value") = py::none());

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

  detail::add_select<holder>(pymesh);
}

}  // namespace pyCGAL::wrap::utils
