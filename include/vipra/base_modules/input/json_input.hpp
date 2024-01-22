#pragma once

#include <filesystem>

#include <fstream>
#include <nlohmann/json.hpp>

#include "vipra/modules.hpp"

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
  auto get(std::string_view key) const -> std::optional<data_t> {
    if (_json.contains(key)) {
      try {
        return _json.at(key).get<data_t>();
      } catch (const nlohmann::json::type_error& e) {
        throw std::runtime_error("Could not parse JSON data at: " + std::string(key) + "\n" + e.what());
      }
    }

    return std::nullopt;
  }

 private:
  nlohmann::json _json;
};
}  // namespace VIPRA::Input