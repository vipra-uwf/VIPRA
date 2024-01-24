#pragma once

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <tuple>
#include <type_traits>
#include <utility>

#include <nlohmann/json.hpp>

#include "vipra/concepts/input.hpp"
#include "vipra/concepts/numeric.hpp"

#include "vipra/modules.hpp"
#include "vipra/types/float.hpp"
#include "vipra/types/parameter.hpp"
#include "vipra/util/get_nth_value.hpp"

#include "vipra/util/debug_do.hpp"
#include "vipra/util/tuple_tail.hpp"

namespace VIPRA::Input {
class JSON {
  VIPRA_MODULE_TYPE(INPUT)
 public:
  explicit JSON(std::string_view path) {
    std::filesystem::path filepath = path;
    if (!std::filesystem::exists(filepath))
      throw std::runtime_error("File does not exist at: " + filepath.string());

    if (!std::filesystem::is_regular_file(filepath)) throw std::runtime_error("File is not a regular file");

    std::ifstream file(filepath);
    if (!file.is_open()) throw std::runtime_error("Could not open file at: " + filepath.string());

    try {
      _json = nlohmann::json::parse(file);
    } catch (const nlohmann::json::parse_error& e) {
      throw std::runtime_error("Could not parse JSON file at: " + filepath.string() + "\n" + e.what());
    }

    file.close();
  }

  template <typename data_t, typename... keys_t>
  [[nodiscard]] auto get(keys_t&&... keys) const -> std::optional<data_t> {
    auto&& keysTuple = std::forward_as_tuple(keys...);

    if constexpr (sizeof...(keys) == 0) {
      Util ::debug_do([&]() {
        std::cout << "Can't Find Empty Key: "
                  << "\n";
      });
      return std::nullopt;
    } else if constexpr (sizeof...(keys) == 1) {
      return get_helper_helper<data_t>(keys..., _json);
    } else {
      return get_helper<data_t>(std::get<0>(keysTuple), Util::tuple_tail(keysTuple), _json);
    }
  }

  template <typename data_t, typename... keys_t>
  [[nodiscard]] auto get_vector(keys_t&&... keys) const -> std::optional<std::vector<data_t>> {
    return get<std::vector<data_t>>(keys...);
  }

 private:
  nlohmann::json _json;

  // NOTE: I don't know if I should be disgusted by this or impressed
  template <typename data_t, typename key_t, typename... keys_t>
  [[nodiscard]] auto get_helper(const key_t baseKey, std::tuple<keys_t...> keys,
                                const nlohmann::json& value) const -> std::optional<data_t> {
    if constexpr (std::tuple_size_v<std::tuple<keys_t...>> == 0) {
      return get_helper_helper<data_t>(baseKey, value);
    } else {
      if (!value.contains(baseKey)) {
        return std::nullopt;
      }
      return get_helper<data_t>(std::get<0>(keys), Util::tuple_tail(keys), value[baseKey]);
    }
  }

  template <typename data_t>
  [[nodiscard]] auto get_helper_helper(const std::string_view key, const nlohmann::json& value) const
      -> std::optional<data_t> {
    if (value.contains(key)) {
      try {
        return value.at(key).get<data_t>();
      } catch (const nlohmann::json::type_error& e) {
        throw std::runtime_error("Could not parse JSON data at: " + std::string(key) + "\n" + e.what());
      }
    }

    return std::nullopt;
  }

  template <Concepts::Numeric data_t>
  [[nodiscard]] inline constexpr auto numeric_parameter_helper(const std::string_view key,
                                                               const nlohmann::json&  value) const
      -> std::optional<Parameter<data_t>> {
    data_t inputData{};
    try {
      if (value[key].is_array()) {
        // discrete, choose random value
        Parameter<data_t> parameter{};
        parameter.randomType = Parameter<data_t>::RandomType::DISCRETE;
        // TODO(rolland): This needs to take a random value, currently just hard coded to 0 till randomization is added
        parameter.value = value[key][0].get<data_t>();
        return parameter;
      }

      if (value[key].is_object()) {
        // range, choose random value
        Parameter<data_t> parameter{};
        parameter.randomType = Parameter<data_t>::RandomType::RANGE;
        // TODO(rolland): This needs to take a random value, currently just hard coded to min till randomization is added
        parameter.value = value[key]["min"].get<data_t>();
        return parameter;
      }

      // single value
      Parameter<data_t> parameter{};
      parameter.randomType = Parameter<data_t>::RandomType::NONE;
      parameter.value = value[key].get<data_t>();
      return parameter;

    } catch (const nlohmann::json::type_error& e) {
      return std::nullopt;
    }
  }
};

template <>
[[nodiscard]] inline auto JSON::get_helper_helper<std::vector<VIPRA::f3d>>(std::string_view      key,
                                                                           const nlohmann::json& value) const
    -> std::optional<std::vector<VIPRA::f3d>> {
  VIPRA::f3dVec inputData{};
  try {
    for (const auto& [subkey, value] : value[key].items()) {
      VIPRA::f3d temp{};
      for (const auto& dimension : value.items()) {
        temp[dimension.key()[0]] = dimension.value().get<VIPRA::f_pnt>();
      }
      inputData.push_back(temp);
    }
  } catch (const nlohmann::json::type_error& e) {
    return std::nullopt;
  }

  if (inputData.empty()) {
    Util ::debug_do([&]() { std::cout << "Can't Find: " << key << "\n"; });
    return std::nullopt;
  }

  return inputData;
}

template <>
[[nodiscard]] inline auto JSON::get_helper_helper<VIPRA::f3d>(std::string_view      key,
                                                              const nlohmann::json& value) const
    -> std::optional<VIPRA::f3d> {
  VIPRA::f3dVec inputData{};
  try {
    for (const auto& [subkey, value] : value[key].items()) {
      VIPRA::f3d temp{};
      for (const auto& dimension : value.items()) {
        temp[dimension.key()[0]] = dimension.value().get<VIPRA::f_pnt>();
      }
      inputData.push_back(temp);
    }
  }

  catch (const nlohmann::json::type_error& e) {
    return std::nullopt;
  }

  if (inputData.empty()) {
    Util ::debug_do([&]() { std::cout << "Can't Find: " << key << "\n"; });
    return std::nullopt;
  }

  return inputData[0];
}

template <>
[[nodiscard]] inline auto JSON::get_helper_helper<Parameter<std::string>>(std::string_view      key,
                                                                          const nlohmann::json& value) const
    -> std::optional<Parameter<std::string>> {
  std::string inputData{};
  try {
    if (value[key].is_array()) {
      // discrete, choose random value
      Parameter<std::string> parameter{};
      parameter.randomType = Parameter<std::string>::RandomType::DISCRETE;
      // TODO(rolland): This needs to take a random value, currently just hard coded to 0 till randomization is added
      parameter.value = value[key][0].get<std::string>();
      return parameter;
    }

    Parameter<std::string> parameter{};
    parameter.randomType = Parameter<std::string>::RandomType::NONE;
    // TODO(rolland): This needs to take a random value, currently just hard coded to 0 till randomization is added
    parameter.value = value[key].get<std::string>();
    return parameter;
  } catch (const nlohmann::json::type_error& e) {
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

// ----------------------- Numeric Parrameter Getters -----------------------

template <>
[[nodiscard]] inline auto JSON::get_helper_helper<Parameter<int>>(std::string_view      key,
                                                                  const nlohmann::json& value) const
    -> std::optional<Parameter<int>> {
  return numeric_parameter_helper<int>(key, value);
}

template <>
[[nodiscard]] inline auto JSON::get_helper_helper<Parameter<float>>(std::string_view      key,
                                                                    const nlohmann::json& value) const
    -> std::optional<Parameter<float>> {
  return numeric_parameter_helper<float>(key, value);
}

template <>
[[nodiscard]] inline auto JSON::get_helper_helper<Parameter<double>>(std::string_view      key,
                                                                     const nlohmann::json& value) const
    -> std::optional<Parameter<double>> {
  return numeric_parameter_helper<double>(key, value);
}

template <>
[[nodiscard]] inline auto JSON::get_helper_helper<Parameter<uint64_t>>(std::string_view      key,
                                                                       const nlohmann::json& value) const
    -> std::optional<Parameter<uint64_t>> {
  return numeric_parameter_helper<uint64_t>(key, value);
}

template <>
[[nodiscard]] inline auto JSON::get_helper_helper<Parameter<uint32_t>>(std::string_view      key,
                                                                       const nlohmann::json& value) const
    -> std::optional<Parameter<uint32_t>> {
  return numeric_parameter_helper<uint32_t>(key, value);
}

template <>
[[nodiscard]] inline auto JSON::get_helper_helper<Parameter<uint16_t>>(std::string_view      key,
                                                                       const nlohmann::json& value) const
    -> std::optional<Parameter<uint16_t>> {
  return numeric_parameter_helper<uint16_t>(key, value);
}

template <>
[[nodiscard]] inline auto JSON::get_helper_helper<Parameter<uint8_t>>(std::string_view      key,
                                                                      const nlohmann::json& value) const
    -> std::optional<Parameter<uint8_t>> {
  return numeric_parameter_helper<uint8_t>(key, value);
}

template <>
[[nodiscard]] inline auto JSON::get_helper_helper<Parameter<int64_t>>(std::string_view      key,
                                                                      const nlohmann::json& value) const
    -> std::optional<Parameter<int64_t>> {
  return numeric_parameter_helper<int64_t>(key, value);
}

template <>
[[nodiscard]] inline auto JSON::get_helper_helper<Parameter<int16_t>>(std::string_view      key,
                                                                      const nlohmann::json& value) const
    -> std::optional<Parameter<int16_t>> {
  return numeric_parameter_helper<int16_t>(key, value);
}

template <>
[[nodiscard]] inline auto JSON::get_helper_helper<Parameter<int8_t>>(std::string_view      key,
                                                                     const nlohmann::json& value) const
    -> std::optional<Parameter<int8_t>> {
  return numeric_parameter_helper<int8_t>(key, value);
}

}  // namespace VIPRA::Input

CHECK_MODULE(InputModule, VIPRA::Input::JSON)