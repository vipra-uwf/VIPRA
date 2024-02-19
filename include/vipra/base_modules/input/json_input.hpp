#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <optional>
#include <tuple>
#include <type_traits>
#include <utility>

#include <nlohmann/json.hpp>

#include "nlohmann/json_fwd.hpp"
#include "vipra/concepts/input.hpp"
#include "vipra/concepts/numeric.hpp"

#include "vipra/concepts/polygon_input.hpp"
#include "vipra/concepts/serializable_input.hpp"
#include "vipra/modules.hpp"
#include "vipra/types/float.hpp"
#include "vipra/types/parameter.hpp"
#include "vipra/util/get_nth_value.hpp"

#include "vipra/util/debug_do.hpp"
#include "vipra/util/has_tag.hpp"
#include "vipra/util/template_specialization.hpp"
#include "vipra/util/tuple_tail.hpp"

namespace VIPRA::Input {
class JSON {
 public:
  VIPRA_MODULE_TYPE(INPUT)
  explicit JSON(std::filesystem::path filepath) : _filepath(std::move(filepath)) {}

  void load();

  template <typename data_t>
  [[nodiscard]] auto get(auto&&... keys) const -> std::optional<data_t>;
  [[nodiscard]] auto load_polygons(auto&&... keys) const -> std::optional<std::vector<Geometry::Polygon>>;

  [[nodiscard]] auto serialize() -> std::string;
  void               deserialize(std::string const& data);

 private:
  nlohmann::json        _json;
  std::filesystem::path _filepath;
  bool                  _loaded{false};

  template <typename... key_ts>
  [[nodiscard]] auto get_value_at_key(key_ts&&... keys) const
      -> std::optional<std::reference_wrapper<const nlohmann::json>>;

  template <typename data_t>
  [[nodiscard]] auto get_value(std::reference_wrapper<const nlohmann::json> const& value) const
      -> std::optional<data_t>;

  [[nodiscard]] static auto is_f3d(nlohmann::json const& value) -> bool;
  [[nodiscard]] static auto is_f2d(nlohmann::json const& value) -> bool;
  [[nodiscard]] static auto get_f3d(std::reference_wrapper<const nlohmann::json> const& value)
      -> std::optional<VIPRA::f3d>;
  [[nodiscard]] static auto get_f3d_vec(std::reference_wrapper<const nlohmann::json> const& value)
      -> std::optional<VIPRA::f3dVec>;

  template <typename data_t>
  [[nodiscard]] auto get_vector(std::reference_wrapper<const nlohmann::json> const& value) const
      -> std::optional<std::vector<data_t>>;

  template <typename data_t>
  [[nodiscard]] static auto numeric_parameter_helper(
      std::reference_wrapper<const nlohmann::json> const& value) -> std::optional<Parameter<data_t>>;

  [[nodiscard]] static inline auto get_parameter(std::reference_wrapper<const nlohmann::json> const& value)
      -> std::optional<Parameter<std::string>>;
};
}  // namespace VIPRA::Input

CHECK_MODULE(InputModule, VIPRA::Input::JSON)
static_assert(VIPRA::Concepts::PolygonInput<VIPRA::Input::JSON>);
static_assert(VIPRA::Concepts::serializable_input<VIPRA::Input::JSON>);

// ---------------------------------------------------- IMPLEMENTATION ---------------------------------------------------- //

/**
   * @brief Returns the value of the given key from the JSON file
   * 
   * @tparam data_t 
   * @tparam key_ts 
   * @param keys 
   * @return std::optional<data_t> 
   */
template <typename data_t, typename... key_ts>
auto VIPRA::Input::JSON::get(key_ts&&... keys) const -> std::optional<data_t> {
  assert(_loaded);

  auto value = get_value_at_key(std::forward<key_ts>(keys)...);
  if (!value) return std::nullopt;

  return get_value<data_t>(value.value());
}

/**
   * @brief Returns polygons from the JSON file
   * @note Requires the JSON for the polygons to be in the format: {"key": [[{"x": 0, "y": 0}, {"x": 1, "y": 1}, ...], ...]} 
   *
   * @tparam data_t 
   * @tparam key_ts 
   * @param key 
   * @return std::vector<Geometry::Polygon> 
   */
template <typename... key_ts>
[[nodiscard]] auto VIPRA::Input::JSON::load_polygons(key_ts&&... keys) const
    -> std::optional<std::vector<Geometry::Polygon>> {
  assert(_loaded);

  auto value = get_value_at_key(std::forward<key_ts>(keys)...);
  if (!value) return std::nullopt;

  std::vector<Geometry::Polygon> polygons;
  for (auto const& polygon : value.value().get()) {
    if (!polygon.is_array()) return std::nullopt;

    std::vector<VIPRA::f3d> points;
    for (auto const& point : polygon) {
      if (!is_f2d(point)) return std::nullopt;
      points.emplace_back(point.at("x").template get<VIPRA::f_pnt>(),
                          point.at("y").template get<VIPRA::f_pnt>(), 0);
    }

    polygons.emplace_back(points);
  }

  return polygons;
}

/**
   * @brief Gets a string parameter from the JSON value
   * 
   * @param value 
   * @return std::optional<Parameter<std::string>> 
   */
[[nodiscard]] auto VIPRA::Input::JSON::get_parameter(
    std::reference_wrapper<const nlohmann::json> const& value) -> std::optional<Parameter<std::string>> {
  std::string inputData{};
  try {
    if (value.get().is_array()) {
      // discrete, choose random value
      Parameter<std::string> parameter{};
      parameter.randomType = Parameter<std::string>::RandomType::DISCRETE;
      // TODO(rolland): This needs to take a random value, currently just hard coded to 0 till randomization is added
      parameter.value = value.get()[0].get<std::string>();
      return parameter;
    }

    Parameter<std::string> parameter{};
    parameter.randomType = Parameter<std::string>::RandomType::NONE;
    // TODO(rolland): This needs to take a random value, currently just hard coded to 0 till randomization is added
    parameter.value = value.get().get<std::string>();
    return parameter;
  } catch (nlohmann::json::type_error const& e) {
    return std::nullopt;
  }

  if (inputData.empty()) {
    return std::nullopt;
  }

  // single value
  Parameter<std::string> parameter{};
  parameter.randomType = Parameter<std::string>::RandomType::NONE;
  parameter.value = inputData;
  return parameter;
}

/**
   * @brief Gets a numeric parameter from the JSON value
   * 
   * @tparam data_t 
   * @param value 
   * @return std::optional<Parameter<data_t>> 
   */
template <typename data_t>
[[nodiscard]] auto VIPRA::Input::JSON::numeric_parameter_helper(
    std::reference_wrapper<const nlohmann::json> const& value) -> std::optional<Parameter<data_t>> {
  data_t inputData{};
  try {
    if (value.get().is_array()) {
      // discrete, choose random value
      Parameter<data_t> parameter{};
      parameter.randomType = Parameter<data_t>::RandomType::DISCRETE;
      // TODO(rolland): This needs to take a random value, currently just hard coded to 0 till randomization is added
      parameter.value = value.get()[0].template get<data_t>();
      return parameter;
    }

    if (value.get().is_object()) {
      // range, choose random value
      Parameter<data_t> parameter{};
      parameter.randomType = Parameter<data_t>::RandomType::RANGE;
      // TODO(rolland): This needs to take a random value, currently just hard coded to min till randomization is added
      parameter.value = value.get()["min"].template get<data_t>();
      return parameter;
    }

    // single value
    Parameter<data_t> parameter{};
    parameter.randomType = Parameter<data_t>::RandomType::NONE;
    parameter.value = value.get().template get<data_t>();
    return parameter;

  } catch (nlohmann::json::type_error const& e) {
    return std::nullopt;
  }
}

/**
   * @brief Gets a vector of data from the JSON value
   * 
   * @tparam data_t 
   * @param value 
   * @return std::optional<std::vector<data_t>> 
   */
template <typename data_t>
[[nodiscard]] auto VIPRA::Input::JSON::get_vector(
    std::reference_wrapper<const nlohmann::json> const& value) const -> std::optional<std::vector<data_t>> {
  assert(_loaded);

  if (!value.get().is_array()) return std::nullopt;

  std::vector<data_t> vec;
  for (auto const& element : value.get()) {
    auto elementValue = get_value<data_t>(std::cref(element));
    if (elementValue) vec.emplace_back(*elementValue);
  }

  return vec;
}

/**
   * @brief Gets an f3dVec from the JSON value
   * 
   * @param value 
   * @return std::optional<VIPRA::f3dVec> 
   */
[[nodiscard]] inline auto VIPRA::Input::JSON::get_f3d_vec(
    std::reference_wrapper<const nlohmann::json> const& value) -> std::optional<VIPRA::f3dVec> {
  if (!value.get().is_array()) return std::nullopt;

  VIPRA::f3dVec f3dVec;
  for (auto const& f3d : value.get()) {
    if (!is_f3d(f3d)) {
      if (!is_f2d(f3d)) return std::nullopt;

      f3dVec.emplace_back(f3d.at("x").get<VIPRA::f_pnt>(), f3d.at("y").get<VIPRA::f_pnt>(), 0);
      continue;
    }

    f3dVec.emplace_back(f3d.at("x").get<VIPRA::f_pnt>(), f3d.at("y").get<VIPRA::f_pnt>(),
                        f3d.at("z").get<VIPRA::f_pnt>());
  }

  return f3dVec;
}

/**
   * @brief Gets an f3d from the JSON value
   * 
   * @param value 
   * @return std::optional<VIPRA::f3d> 
   */
[[nodiscard]] inline auto VIPRA::Input::JSON::get_f3d(
    std::reference_wrapper<const nlohmann::json> const& value) -> std::optional<VIPRA::f3d> {
  if (!is_f3d(value.get())) return std::nullopt;

  return VIPRA::f3d(value.get().at("x").get<VIPRA::f_pnt>(), value.get().at("y").get<VIPRA::f_pnt>(),
                    value.get().at("z").get<VIPRA::f_pnt>());
}

/**
   * @brief Loads the JSON file from the given path
   * 
   * @param filepath 
   */
inline void VIPRA::Input::JSON::load() {
  if (_loaded) return;

  if (!std::filesystem::exists(_filepath))
    throw std::runtime_error("File does not exist at: " + _filepath.string());

  if (!std::filesystem::is_regular_file(_filepath))
    throw std::runtime_error("File is not a regular file at: " + _filepath.string());

  std::ifstream file(_filepath, std::ios::in);
  if (!file.is_open()) throw std::runtime_error("Could not open file at: " + _filepath.string());

  try {
    _json = nlohmann::json::parse(file);
  } catch (nlohmann::json::parse_error const& e) {
    throw std::runtime_error("Could not parse JSON file at: " + _filepath.string() + "\n" + e.what());
  }

  _loaded = true;
  file.close();
}

template <typename... key_ts>
[[nodiscard]] auto VIPRA::Input::JSON::get_value_at_key(key_ts&&... keys) const
    -> std::optional<std::reference_wrapper<const nlohmann::json>> {
  auto value = std::cref(_json);
  bool found = true;

  auto const findKey = [&](std::string const& key) {
    if (!found) return;

    if (!value.get().contains(key)) {
      found = false;
      return;
    }

    value = std::cref(value.get().at(key));
  };

  (findKey(keys), ...);

  if (!found) return std::nullopt;

  return value;
}

/**
   * @brief Routes the value to the correct type getter, if possible, otherwise returns std::nullopt
   * 
   * @tparam data_t 
   * @param value 
   * @return std::optional<data_t> 
   */
template <typename data_t>
[[nodiscard]] auto VIPRA::Input::JSON::get_value(
    std::reference_wrapper<const nlohmann::json> const& value) const -> std::optional<data_t> {
  if constexpr (std::is_same_v<data_t, VIPRA::Parameter<std::string>>) {
    return get_parameter(value);
  }

  else if constexpr (std::is_same_v<data_t, VIPRA::f3d>) {
    return get_f3d(value);
  }

  else if constexpr (std::is_same_v<data_t, VIPRA::f3dVec>) {
    return get_f3d_vec(value);
  }

  else if constexpr (Util::is_specialization<data_t, std::vector>::value) {
    using value_t = typename Util::get_specialization_internal<data_t>::type;
    return get_vector<value_t>(value);
  }

  else if constexpr (Util::is_specialization<data_t, Parameter>::value) {
    using value_t = typename Util::get_specialization_internal<data_t>::type;
    return numeric_parameter_helper<value_t>(value);
  } else {
    try {
      return value.get().get<data_t>();
    } catch (nlohmann::json::type_error const& e) {
      return std::nullopt;
    }
  }

  return std::nullopt;
}

/**
   * @brief Checks if the JSON value is a valid f3d
   * 
   * @param value 
   * @return true 
   * @return false 
   */
[[nodiscard]] inline auto VIPRA::Input::JSON::is_f3d(nlohmann::json const& value) -> bool {
  if (!value.is_object()) return false;
  if (!value.contains("x") || !value.contains("y") || !value.contains("z")) return false;
  if (!value.at("x").is_number() || !value.at("y").is_number() || !value.at("z").is_number()) return false;
  return true;
}

/**
   * @brief Checks if the JSON value is a valid f3d, with only x and y
   * 
   * @param value 
   * @return true 
   * @return false 
   */
[[nodiscard]] inline auto VIPRA::Input::JSON::is_f2d(nlohmann::json const& value) -> bool {
  if (!value.is_object()) return false;
  if (!value.contains("x") || !value.contains("y")) return false;
  if (!value.at("x").is_number() || !value.at("y").is_number()) return false;
  return true;
}

inline auto VIPRA::Input::JSON::serialize() -> std::string { return _json.dump(); }

inline void VIPRA::Input::JSON::deserialize(std::string const& data) {
  try {
    _json = nlohmann::json::parse(data);
    _loaded = true;
  } catch (nlohmann::json::parse_error const& e) {
    throw std::runtime_error("Could not parse JSON data\n");
  }
}
