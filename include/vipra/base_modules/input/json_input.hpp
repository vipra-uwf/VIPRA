#pragma once

#include <filesystem>

#include <fstream>
#include <nlohmann/json.hpp>

#include "vipra/concepts/input.hpp"
#include "vipra/modules.hpp"
#include "vipra/types/float.hpp"

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

  template <typename data_t>
  [[nodiscard]] auto get(std::string_view key) const -> std::optional<data_t> {
    if (_json.contains(key)) {
      try {
        return _json.at(key).get<data_t>();
      } catch (const nlohmann::json::type_error& e) {
        throw std::runtime_error("Could not parse JSON data at: " + std::string(key) + "\n" + e.what());
      }
    }

    return std::nullopt;
  }

  template <typename data_t>
  [[nodiscard]] auto get_vector(std::string_view key) const -> std::optional<std::vector<data_t>> {
    if (_json.contains(key)) {
      try {
        return _json.at(key).get<std::vector<data_t>>();
      } catch (const nlohmann::json::type_error& e) {
        throw std::runtime_error("Could not parse JSON data at: " + std::string(key) + "\n" + e.what());
      }
    }

    return std::nullopt;
  }

 private:
  nlohmann::json _json;
};

template <>
inline auto JSON::get_vector<VIPRA::f3d>(std::string_view key) const
    -> std::optional<std::vector<VIPRA::f3d>> {
  VIPRA::f3dVec inputData{};
  try {
    for (const auto& [subkey, value] : _json[key].items()) {
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
    return std::nullopt;
  }

  return inputData;
}

template <>
inline auto JSON::get<VIPRA::f3d>(std::string_view key) const -> std::optional<VIPRA::f3d> {
  VIPRA::f3dVec inputData{};
  try {
    for (const auto& [subkey, value] : _json[key].items()) {
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
    return std::nullopt;
  }

  return inputData[0];
}
}  // namespace VIPRA::Input

CHECK_MODULE(InputModule, VIPRA::Input::JSON)