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

#include "vipra/concepts/string_view.hpp"

#include "vipra/modules/input.hpp"

#include "vipra/modules/param_reader.hpp"
#include "vipra/modules/polygon_input.hpp"
#include "vipra/modules/serializable.hpp"

#include "vipra/geometry/polygon.hpp"

#include "vipra/random/distributions.hpp"

#include "vipra/util/is_map.hpp"
#include "vipra/util/template_specialization.hpp"

#include "vipra/macros/module.hpp"

namespace VIPRA::Input {
/**
  * @brief Parameter and Polygon qualified JSON input module
  * 
  * 
  */
class JSON : public VIPRA::Modules::Module<JSON>,
             public VIPRA::Modules::Input<JSON>,
             public VIPRA::Modules::ParamReader<JSON>,
             public VIPRA::Modules::Serializable<JSON>,
             public VIPRA::Modules::PolygonInput<JSON> {
 public:
  VIPRA_MODULE_NAME("json");
  VIPRA_MODULE_TYPE(INPUT);

  explicit JSON(std::filesystem::path filepath) : _filepath(std::move(filepath)) {}

  void load_impl();

  template <typename data_t, Concepts::StringView... keys_t>
  [[nodiscard]] auto get(keys_t&&... keys) const -> std::optional<data_t>;

 private:
  nlohmann::json        _json;
  std::filesystem::path _filepath;

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
  [[nodiscard]] auto get_map(std::reference_wrapper<const nlohmann::json> const& value) const
      -> std::optional<std::map<std::string, data_t>>;

  void parse_impl(std::string const& data) {
    try {
      _json = nlohmann::json::parse(data);
      set_loaded(true);
    } catch ( nlohmann::json::parse_error const& e ) {
      throw std::runtime_error("Could not parse JSON data\n");
    }
  }
};
}  // namespace VIPRA::Input

// ---------------------------------------------------- IMPLEMENTATION ---------------------------------------------------- //

/**
   * @brief Returns the value of the given key from the JSON file
   * 
   * @tparam data_t 
   * @tparam key_ts 
   * @param keys 
   * @return std::optional<data_t> 
   */
template <typename data_t, VIPRA::Concepts::StringView... key_ts>
auto VIPRA::Input::JSON::get(key_ts&&... keys) const -> std::optional<data_t> {
  auto value = get_value_at_key(std::forward<key_ts>(keys)...);
  if ( ! value ) return std::nullopt;

  return get_value<data_t>(value.value());
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
  if ( ! value.get().is_array() ) return std::nullopt;

  std::vector<data_t> vec;
  for ( auto const& element : value.get() ) {
    auto elementValue = get_value<data_t>(std::cref(element));
    if ( elementValue ) vec.emplace_back(*elementValue);
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
  if ( ! value.get().is_array() ) return std::nullopt;

  VIPRA::f3dVec f3dVec;
  for ( auto const& f3d : value.get() ) {
    if ( ! is_f3d(f3d) ) {
      if ( ! is_f2d(f3d) ) return std::nullopt;

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
  if ( ! is_f3d(value.get()) ) return std::nullopt;

  return VIPRA::f3d(value.get().at("x").get<VIPRA::f_pnt>(), value.get().at("y").get<VIPRA::f_pnt>(),
                    value.get().at("z").get<VIPRA::f_pnt>());
}

/**
   * @brief Loads the JSON file from the given path
   * 
   * @param filepath 
   */
inline void VIPRA::Input::JSON::load_impl() {
  if ( ! std::filesystem::exists(_filepath) )
    throw std::runtime_error("File does not exist at: " + _filepath.string());

  if ( ! std::filesystem::is_regular_file(_filepath) )
    throw std::runtime_error("File is not a regular file at: " + _filepath.string());

  std::ifstream file(_filepath, std::ios::in);
  if ( ! file.is_open() ) throw std::runtime_error("Could not open file at: " + _filepath.string());

  try {
    _json = nlohmann::json::parse(file);
  } catch ( nlohmann::json::parse_error const& e ) {
    throw std::runtime_error("Could not parse JSON file at: " + _filepath.string() + "\n" + e.what());
  }

  file.close();
}

/**
 * @brief Returns the json value at the provided key location
 * 
 * @tparam key_ts 
 * @param keys 
 * @return std::optional<std::reference_wrapper<const nlohmann::json>> 
 */
template <typename... key_ts>
[[nodiscard]] auto VIPRA::Input::JSON::get_value_at_key(key_ts&&... keys) const
    -> std::optional<std::reference_wrapper<const nlohmann::json>> {
  auto value = std::cref(_json);
  bool found = true;

  auto const findKey = [&](std::string const& key) {
    if ( ! found ) return;

    if ( ! value.get().contains(key) ) {
      found = false;
      return;
    }

    value = std::cref(value.get().at(key));
  };

  // drill through each key to get the final value
  (findKey(keys), ...);

  if ( ! found ) return std::nullopt;

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
  if constexpr ( std::is_same_v<data_t, VIPRA::f3d> ) {
    return get_f3d(value);
  }

  else if constexpr ( std::is_same_v<data_t, VIPRA::f3dVec> ) {
    return get_f3d_vec(value);
  }

  else if constexpr ( VIPRA::Util::is_specialization<data_t, std::vector>::value ) {
    using value_t = typename VIPRA::Util::get_specialization_internal<data_t>::type;
    return get_vector<value_t>(value);
  }

  else if constexpr ( VIPRA::Util::is_map_v<data_t> ) {
    using value_t = typename VIPRA::Util::get_map_specialization<data_t>::value_t;
    return get_map<value_t>(value);
  }

  else {
    try {
      return value.get().get<data_t>();
    } catch ( nlohmann::json::type_error const& e ) {
      return std::nullopt;
    }
  }

  return std::nullopt;
}

template <typename data_t>
[[nodiscard]] auto VIPRA::Input::JSON::get_map(std::reference_wrapper<const nlohmann::json> const& value)
    const -> std::optional<std::map<std::string, data_t>> {
  try {
    return value.get().get<std::map<std::string, data_t>>();
  } catch ( nlohmann::json::type_error const& e ) {
    return std::nullopt;
  }
}

/**
   * @brief Checks if the JSON value is a valid f3d
   * 
   * @param value 
   * @return true 
   * @return false 
   */
[[nodiscard]] inline auto VIPRA::Input::JSON::is_f3d(nlohmann::json const& value) -> bool {
  if ( ! value.is_object() ) return false;
  if ( ! value.contains("x") || ! value.contains("y") || ! value.contains("z") ) return false;
  if ( ! value.at("x").is_number() || ! value.at("y").is_number() || ! value.at("z").is_number() )
    return false;
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
  if ( ! value.is_object() ) return false;
  if ( ! value.contains("x") || ! value.contains("y") ) return false;
  if ( ! value.at("x").is_number() || ! value.at("y").is_number() ) return false;
  return true;
}