#pragma once

#include <filesystem>

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <utility>

#include "vipra/concepts/input.hpp"
#include "vipra/modules.hpp"
#include "vipra/types/float.hpp"
#include "vipra/util/get_nth_value.hpp"

#include "vipra/util/debug_do.hpp"

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
  [[nodiscard]] auto get(keys_t... keys) const -> std::optional<data_t> {
    if constexpr (sizeof...(keys) == 0) {
      Util ::debug_do([&]() {
        std::cout << "Can't Find Empty Key: "
                  << "\n";
      });
      return std::nullopt;
    } else if constexpr (sizeof...(keys) == 1) {
      return get_helper_helper<data_t>(keys..., _json);
    } else {
      return get_helper<data_t>(Util::get_nth_value<0>(keys...), keys...);
    }
  }

  template <typename data_t, typename... keys_t>
  [[nodiscard]] auto get_vector(keys_t... keys) const -> std::optional<std::vector<data_t>> {
    if constexpr (sizeof...(keys) == 0) {
      Util ::debug_do([&]() {
        std::cout << "Can't Find Empty Key:"
                  << "\n";
      });
      return std::nullopt;
    } else if constexpr (sizeof...(keys) == 1) {
      return get_helper_helper<std::vector<data_t>>(keys..., _json);
    } else {
      return get_helper<std::vector<data_t>>(Util::get_nth_value<0>(keys...), keys...);
    }
  }

 private:
  nlohmann::json _json;

  // NOTE: I don't know if I should be disgusted by this or impressed
  template <typename data_t, typename key_t, typename... keys_t>
  [[nodiscard]] auto get_helper(key_t baseKey, keys_t... keys) const -> std::optional<data_t> {
    Util::debug_do([&]() { std::cout << "get_helper: " << baseKey << "\n"; });

    if constexpr (sizeof...(keys) == 0) {
      return get_helper<data_t>(baseKey, _json);
    } else {
      return get_helper_helper<data_t>(Util::get_nth_value<0>(keys...), _json[baseKey]);
    }
  }

  template <typename data_t>
  [[nodiscard]] auto get_helper_helper(std::string_view key, const nlohmann::json& value) const
      -> std::optional<data_t> {
    Util::debug_do([&]() { std::cout << "get_helper_helper: " << key << "\n"; });

    if (value.contains(key)) {
      try {
        return value.at(key).get<data_t>();
      } catch (const nlohmann::json::type_error& e) {
        throw std::runtime_error("Could not parse JSON data at: " + std::string(key) + "\n" + e.what());
      }
    }

    Util::debug_do([&]() { std::cout << "Can't Find: " << key << "\n"; });
    return std::nullopt;
  }
};

template <>
inline auto JSON::get_helper_helper<std::vector<VIPRA::f3d>>(std::string_view      key,
                                                             const nlohmann::json& value) const
    -> std::optional<std::vector<VIPRA::f3d>> {
  Util::debug_do([&]() { std::cout << "get_helper_helper std::vector<VIPRA::f3d>: " << key << "\n"; });

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
    Util::debug_do([&]() { std::cout << "Can't Find: " << key << "\n"; });
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
  Util::debug_do([&]() { std::cout << "get_helper_helper VIPRA::f3d: " << key << "\n"; });

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
    Util::debug_do([&]() { std::cout << "Can't Find: " << key << "\n"; });
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