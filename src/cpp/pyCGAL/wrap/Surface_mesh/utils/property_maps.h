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
  using Holder = Pmap_holder<Surface_mesh, Index, property_alternatives>;
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
        [i](auto alternative) -> py::object {
          if constexpr (std::is_same_v<
                            typename decltype(alternative)::value_type, bool>) {
            return py::bool_{alternative[i]};
          } else {
            return py::cast(alternative[i]);
          }
          assert(false);
          return py::none{};
        },
        map);
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
            return false;  // visit() requires the result of all potential
                           // invocations to have the same type and value
                           // category
          }
        },
        map);
  }
  void copy(const Index source, const Index target) {
    std::visit(
        [source, target](auto alternative) {
          alternative[target] = alternative[source];
        },
        map);
  }
  void copy(const std::vector<Index>& sources,
            const std::vector<Index>& targets) {
    std::visit(
        [&sources, &targets](auto alternative) {
          if (sources.size() != targets.size()) {
            throw std::runtime_error(
                "There must be as many source as target indices.");
          }
          auto target = begin(targets);
          for (auto&& source : sources) {
            alternative[*target] = alternative[source];
            ++target;
          }
        },
        map);
  }
  void copy(py::sequence sources, py::sequence targets) {
    std::visit(
        [&sources, &targets](auto alternative) {
          if (py::len(sources) != py::len(targets)) {
            throw std::runtime_error(
                "There must be as many source as target indices.");
          }
          auto target = std::begin(targets);
          for (auto&& source : sources) {
            alternative[target->cast<Index>()] =
                alternative[source.cast<Index>()];
            ++target;
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
  void set(py::list& indices, py::iterable& values) {
    std::visit(
        [&indices, &values](auto alternative) {
          using value_type = typename decltype(alternative)::value_type;
          auto pi = std::begin(indices);
          for (auto&& value : values) {
            assert(pi != std::end(indices));
            alternative[pi->cast<Index>()] = value.cast<value_type>();
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
  void set(py::list& indices, py::object& value) {
    std::visit(
        [&indices, &value](auto alternative) {
          using value_type = typename decltype(alternative)::value_type;
          const auto v = value.cast<value_type>();
          for (auto&& i : indices) {
            alternative[i.cast<Index>()] = v;
          }
        },
        map);
  }
  void set(const Holder& where, py::object& value) {
    std::visit(
        [&value](auto self, auto mask) {
          using value_type = typename decltype(self)::value_type;
          if constexpr (std::is_same_v<value_type, bool>) {
            throw std::runtime_error(
                "Use logical operators to set boolean maps.");
          }
          if constexpr (!std::is_same_v<typename decltype(mask)::value_type,
                                        bool>) {
            throw std::runtime_error(
                "Location must be given as a boolean property.");
          }
          const auto v = value.cast<value_type>();
          auto p = self.begin();
          for (auto&& ok : mask) {
            if (ok) (*p) = v;
            ++p;
          }
          assert(p == self.end());
        },
        map, where.map);
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
  void logical_or(const Holder& holder) {
    return std::visit(
        [](auto self, auto other) {
          if constexpr (std::is_same_v<typename decltype(self)::value_type,
                                       bool> &&
                        std::is_same_v<typename decltype(other)::value_type,
                                       bool>) {
            auto p = other.begin();
            // we must use forward reference with std::vector<bool>
            for (auto&& b : self) {
              assert(p != other.end());
              b = b || *p;  // *p is not a reference to a bit
              ++p;
            }
            assert(p == other.end());
          } else {
            throw std::runtime_error("Property maps must hold boolean types.");
          }
        },
        map, holder.map);
  }
  void logical_and(const Holder& holder) {
    return std::visit(
        [](auto self, auto other) {
          if constexpr (std::is_same_v<typename decltype(self)::value_type,
                                       bool> &&
                        std::is_same_v<typename decltype(other)::value_type,
                                       bool>) {
            auto p = other.begin();
            // we must use forward reference with std::vector<bool>
            for (auto&& b : self) {
              assert(p != other.end());
              b = b && *p;  // *p is not a reference to a bit
              ++p;
            }
            assert(p == other.end());
          } else {
            throw std::runtime_error("Property maps must hold boolean types.");
          }
        },
        map, holder.map);
  }
  void logical_xor(const Holder& holder) {
    return std::visit(
        [](auto self, auto other) {
          if constexpr (std::is_same_v<typename decltype(self)::value_type,
                                       bool> &&
                        std::is_same_v<typename decltype(other)::value_type,
                                       bool>) {
            auto p = other.begin();
            // we must use forward reference with std::vector<bool>
            for (auto&& b : self) {
              assert(p != other.end());
              b = (b != *p);  // *p is not a reference to a bit
              ++p;
            }
            assert(p == other.end());
          } else {
            throw std::runtime_error("Property maps must hold boolean types.");
          }
        },
        map, holder.map);
  }
  void flip() {
    return std::visit(
        [](auto alternative) {
          if constexpr (std::is_same_v<
                            typename decltype(alternative)::value_type, bool>) {
            // we must use forward reference with std::vector<bool>
            for (auto&& b : alternative) {
              b = !b;
            }
          } else {
            throw std::runtime_error("Property map must hold boolean types.");
          }
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
        [](auto&& alternative) -> py::iterator {
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
};  // namespace detail

// Compiler error with MSVC 2019
// template <typename T, typename Holder>
// inline auto get_map(const Holder& pmap) -> const
//    typename Holder::template Property_map& {
//  auto p = pmap.template get_underlying_map<T>();
//  assert(p);
//  return *p;
//}

// WIP
// template <typename Surface_mesh>
// struct Visitor {
//  template <typename Index, typename T>
//  void operator()(
//      const typename Surface_mesh::template Property_map<Index, T>&
//          map) const {};
//};

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
  template <typename Holder, typename Surface_mesh>
  static void keep(py::class_<Surface_mesh>& pymesh) {
    using Index = typename Holder::Property_index;
    pymesh.def(
        "keep", [](const Surface_mesh& mesh, std::vector<Index>& selection,
                   const Holder& pmap, T value) {
          auto p = pmap.template get_underlying_map<T>();
          assert(p);
          keep_indices<Surface_mesh, Index, T>(mesh, selection, *p, value);
        });
    if constexpr (sizeof...(Ts) > 0) {
      Add<std::tuple<Ts...>>::template keep<Holder>(pymesh);
    }
  }
  template <typename Holder, typename Surface_mesh>
  static void extract(py::class_<Surface_mesh>& pymesh) {
    using Index = typename Holder::Property_index;
    pymesh.def("extract",
               [](const Surface_mesh& mesh, const std::vector<Index>& selection,
                  const Holder& pmap, T value) {
                 auto p = pmap.template get_underlying_map<T>();
                 assert(p);
                 return extract_indices<Surface_mesh, Index, T>(mesh, selection,
                                                                *p, value);
               });
    if constexpr (sizeof...(Ts) > 0) {
      Add<std::tuple<Ts...>>::template extract<Holder>(pymesh);
    }
  }
  template <typename Holder, typename Surface_mesh>
  static void select_edges(py::class_<Surface_mesh>& pymesh) {
    using Index = typename Holder::Property_index;
    pymesh.def("select_edges",
               [](const Surface_mesh& mesh, const Holder& pmap, T value) {
                 auto p = pmap.template get_underlying_map<T>();
                 assert(p);
                 return select_edge_indices<Surface_mesh, T>(mesh, *p, value);
               });
    if constexpr (sizeof...(Ts) > 0) {
      Add<std::tuple<Ts...>>::template select_edges<Holder>(pymesh);
    }
  }
};

template <typename Holder, typename Surface_mesh>
void add_selections(py::class_<Surface_mesh>& pymesh) {
  using Index = typename Holder::Property_index;
  using alternatives = typename Holder::property_alternatives;
  Add<alternatives>::template select<Holder>(pymesh);
  Add<alternatives>::template keep<Holder>(pymesh);
  Add<alternatives>::template extract<Holder>(pymesh);
  if constexpr (std::is_same_v<Index, typename Surface_mesh::Vertex_index>) {
    Add<alternatives>::template select_edges<Holder>(pymesh);
    pymesh.def("select_crossing_edges", [](const Surface_mesh& mesh,
                                           const Holder& pmap) {
      return std::visit(
          [&](const auto& map) { select_crossing_edge_indices(mesh, map); },
          pmap.map);
    });
    pymesh.def(
        "select_crossing_edges",
        [](const Surface_mesh& mesh, const Holder& pmap, const double value) {
          std::visit(
              [&](const auto& map) {
                select_crossing_isovalue_edge_indices(mesh, map, value);
              },
              pmap.map);
        });
    pymesh.def("keep_crossing_edges",
               [](const Surface_mesh& mesh,
                  std::vector<typename Surface_mesh::Edge_index>& selection,
                  const Holder& pmap) {
                 std::visit(
                     [&](const auto& map) {
                       keep_crossing_edge_indices(mesh, selection, map);
                     },
                     pmap.map);
               });
    pymesh.def("keep_crossing_edges",
               [](const Surface_mesh& mesh,
                  std::vector<typename Surface_mesh::Edge_index>& selection,
                  const Holder& pmap, const double value) {
                 std::visit(
                     [&](const auto& map) {
                       keep_crossing_isovalue_edge_indices(mesh, selection, map,
                                                           value);
                     },
                     pmap.map);
               });
    pymesh.def(
        "collect_edges", [](const Surface_mesh& mesh, const Holder& pmap) {
          return std::visit(
              [&](auto&& map) { return collect_edge_indices(mesh, map); },
              pmap.map);
        });
  }
}

template <typename SurfaceMesh, typename IndexType>
struct Helper_traits {
  using Surface_mesh = SurfaceMesh;
  using index = IndexType;
  using alternatives = pyCGAL::available_property_types;
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
    if (tmp) return Result{typename Traits::holder{*tmp}};
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
  pyholder.def("copy",
               py::overload_cast<const Index, const Index>(&holder::copy));
  pyholder.def(
      "copy",
      py::overload_cast<const std::vector<Index>&, const std::vector<Index>&>(
          &holder::copy));
  pyholder.def("copy",
               py::overload_cast<py::sequence, py::sequence>(&holder::copy));
  pyholder.def("set", py::overload_cast<Index, py::object&>(&holder::set));
  pyholder.def("set",
               py::overload_cast<const std::vector<Index>&, py::iterable&>(
                   &holder::set));
  pyholder.def("set", py::overload_cast<const std::vector<Index>&, py::object&>(
                          &holder::set));
  pyholder.def("set",
               py::overload_cast<py::list&, py::iterable&>(&holder::set));
  pyholder.def("set", py::overload_cast<py::list&, py::object&>(&holder::set));
  pyholder.def("set",
               py::overload_cast<const holder&, py::object&>(&holder::set));
  pyholder.def("set", &holder::fill);
  pyholder.def("fill", &holder::fill);
  pyholder.def("logical_or", &holder::logical_or);
  pyholder.def("logical_and", &holder::logical_and);
  pyholder.def("logical_xor", &holder::logical_xor);
  pyholder.def("flip", &holder::flip);
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

  detail::add_selections<holder>(pymesh);
}

}  // namespace pyCGAL::wrap::utils
