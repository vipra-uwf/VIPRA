#pragma once

#include <filesystem>

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <tuple>
#include <utility>

#include "vipra/concepts/input.hpp"
#include "vipra/modules.hpp"
#include "vipra/types/float.hpp"
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
};

template <>
inline auto JSON::get_helper_helper<std::vector<VIPRA::f3d>>(std::string_view      key,
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
inline auto JSON::get_helper_helper<VIPRA::f3d>(std::string_view key, const nlohmann::json& value) const
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
}  // namespace VIPRA::Input

CHECK_MODULE(InputModule, VIPRA::Input::JSON)