#pragma once

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <optional>
#include <string>
#include <type_traits>

#include <nlohmann/json.hpp>

#include "vipra/geometry/f3d.hpp"

#include "vipra/modules/map_input.hpp"
#include "vipra/modules/param_reader.hpp"
#include "vipra/modules/pedestrian_input.hpp"

#include "vipra/geometry/polygon.hpp"

#include "vipra/modules/serializable.hpp"
#include "vipra/util/is_map.hpp"
#include "vipra/util/template_specialization.hpp"

namespace VIPRA::Input {
/**
  * @brief Parameter and Polygon qualified JSON input module
  * 
  * 
  */
class JSON : public VIPRA::Modules::ParamReader<JSON>,
             public VIPRA::Modules::Serializable<JSON>,
             public VIPRA::Modules::MapInput,
             public VIPRA::Modules::PedestrianInput {
  using json_cref = std::reference_wrapper<const nlohmann::json>;

 public:
  void load(std::string const& filepath) override;

  template <typename data_t>
  [[nodiscard]] auto get(std::vector<std::string> const& keys) const
      -> std::optional<data_t>;

  [[nodiscard]] auto to_string() -> std::string { return _json.dump(); }

  [[nodiscard]] auto get_pedestrians() const
      -> std::optional<VIPRA::f3dVec> override;
  [[nodiscard]] auto get_obstacles() const
      -> std::optional<std::vector<VIPRA::Geometry::Polygon>> override;
  [[nodiscard]] auto get_spawns() const
      -> std::optional<std::vector<VIPRA::Geometry::Polygon>> override;
  [[nodiscard]] auto get_objectives() const
      -> std::optional<std::map<
          std::string, std::vector<VIPRA::Geometry::Polygon>>> override;
  [[nodiscard]] auto get_areas() const
      -> std::optional<
          std::map<std::string, VIPRA::Geometry::Polygon>> override;

 private:
  nlohmann::json _json;

  template <typename data_t>
  [[nodiscard]] auto get_value(json_cref const& value) const
      -> std::optional<data_t>;

  [[nodiscard]] static auto is_f3d(nlohmann::json const& value) -> bool;
  [[nodiscard]] static auto is_f2d(nlohmann::json const& value) -> bool;
  [[nodiscard]] static auto get_f3d(json_cref const& value)
      -> std::optional<VIPRA::f3d>;
  [[nodiscard]] static auto get_f3d_vec(json_cref const& value)
      -> std::optional<VIPRA::f3dVec>;

  template <typename data_t>
  [[nodiscard]] auto get_vector(json_cref const& value) const
      -> std::optional<std::vector<data_t>>;

  template <typename data_t>
  [[nodiscard]] auto get_map(json_cref const& value) const
      -> std::optional<std::map<std::string, data_t>>;

  [[nodiscard]] static auto get_polygons(json_cref const& value)
      -> std::optional<std::vector<VIPRA::Geometry::Polygon>>;

  [[nodiscard]] auto get_value_at_key(
      std::vector<std::string> const& keys) const -> std::optional<json_cref>
  {
    auto value = std::cref(_json);
    bool found = true;

    std::for_each(keys.begin(), keys.end(), [&](std::string const& key) {
      if ( ! found ) return;

      if ( ! value.get().contains(key) ) {
        found = false;
        return;
      }

      value = std::cref(value.get().at(key));
    });

    if ( ! found ) return std::nullopt;

    return value;
  }

 public:
  void parse(std::string const& data)
  {
    try {
      _json = nlohmann::json::parse(data);
    }
    catch ( nlohmann::json::parse_error const& e ) {
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
template <typename data_t>
auto VIPRA::Input::JSON::get(std::vector<std::string> const& keys) const
    -> std::optional<data_t>
{
  auto value = get_value_at_key(keys);
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
[[nodiscard]] auto VIPRA::Input::JSON::get_vector(json_cref const& value) const
    -> std::optional<std::vector<data_t>>
{
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
    json_cref const& value) -> std::optional<VIPRA::f3dVec>
{
  if ( ! value.get().is_array() ) return std::nullopt;

  VIPRA::f3dVec f3dVec;
  for ( auto const& f3d : value.get() ) {
    if ( ! is_f3d(f3d) ) {
      if ( ! is_f2d(f3d) ) return std::nullopt;

      f3dVec.emplace_back(f3d.at("x").get<VIPRA::f_pnt>(),
                          f3d.at("y").get<VIPRA::f_pnt>(), 0);
      continue;
    }

    f3dVec.emplace_back(f3d.at("x").get<VIPRA::f_pnt>(),
                        f3d.at("y").get<VIPRA::f_pnt>(),
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
[[nodiscard]] inline auto VIPRA::Input::JSON::get_f3d(json_cref const& value)
    -> std::optional<VIPRA::f3d>
{
  if ( ! is_f3d(value.get()) ) return std::nullopt;

  return VIPRA::f3d(value.get().at("x").get<VIPRA::f_pnt>(),
                    value.get().at("y").get<VIPRA::f_pnt>(),
                    value.get().at("z").get<VIPRA::f_pnt>());
}

/**
   * @brief Loads the JSON file from the given path
   * 
   * @param filepath 
   */
inline void VIPRA::Input::JSON::load(std::string const& filepath)
{
  if ( ! std::filesystem::exists(filepath) )
    throw std::runtime_error("File does not exist at: " + filepath);

  if ( ! std::filesystem::is_regular_file(filepath) )
    throw std::runtime_error("File is not a regular file at: " + filepath);

  std::ifstream file(filepath, std::ios::in);
  if ( ! file.is_open() )
    throw std::runtime_error("Could not open file at: " + filepath);

  try {
    _json = nlohmann::json::parse(file);
  }
  catch ( nlohmann::json::parse_error const& e ) {
    throw std::runtime_error("Could not parse JSON file at: " + filepath +
                             "\n" + e.what());
  }

  file.close();
}

/**
   * @brief Routes the value to the correct type getter, if possible, otherwise returns std::nullopt
   * 
   * @tparam data_t 
   * @param value 
   * @return std::optional<data_t> 
   */
template <typename data_t>
[[nodiscard]] auto VIPRA::Input::JSON::get_value(json_cref const& value) const
    -> std::optional<data_t>
{
  if constexpr ( std::is_same_v<data_t, VIPRA::f3d> ) {
    return get_f3d(value);
  }

  else if constexpr ( std::is_same_v<data_t, VIPRA::f3dVec> ) {
    return get_f3d_vec(value);
  }

  else if constexpr ( std::is_same_v<data_t,
                                     std::vector<VIPRA::Geometry::Polygon>> ) {
    return get_polygons(value);
  }

  else if constexpr ( VIPRA::Util::is_specialization<data_t,
                                                     std::vector>::value ) {
    using value_t =
        typename VIPRA::Util::get_specialization_internal<data_t>::type;
    return get_vector<value_t>(value);
  }

  else if constexpr ( VIPRA::Util::is_map_v<data_t> ) {
    using value_t =
        typename VIPRA::Util::get_map_specialization<data_t>::value_t;
    return get_map<value_t>(value);
  }

  else {
    try {
      return value.get().get<data_t>();
    }
    catch ( nlohmann::json::type_error const& e ) {
      return std::nullopt;
    }
  }

  return std::nullopt;
}

template <typename data_t>
auto VIPRA::Input::JSON::get_map(json_cref const& value) const
    -> std::optional<std::map<std::string, data_t>>
{
  try {
    return value.get().get<std::map<std::string, data_t>>();
  }
  catch ( nlohmann::json::type_error const& e ) {
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
[[nodiscard]] inline auto VIPRA::Input::JSON::is_f3d(
    nlohmann::json const& value) -> bool
{
  if ( ! value.is_object() ) return false;
  if ( ! value.contains("x") || ! value.contains("y") || ! value.contains("z") )
    return false;
  if ( ! value.at("x").is_number() || ! value.at("y").is_number() ||
       ! value.at("z").is_number() )
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
[[nodiscard]] inline auto VIPRA::Input::JSON::is_f2d(
    nlohmann::json const& value) -> bool
{
  if ( ! value.is_object() ) return false;
  if ( ! value.contains("x") || ! value.contains("y") ) return false;
  if ( ! value.at("x").is_number() || ! value.at("y").is_number() )
    return false;
  return true;
}