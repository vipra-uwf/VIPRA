#pragma once

#include <string>

namespace VIPRA::Modules {

class Serializable {
 public:
  [[nodiscard]] virtual auto serialize() -> std::string = 0;
  virtual void               parse(std::string const& data) = 0;

  Serializable() = default;
  Serializable(const Serializable&) = default;
  Serializable(Serializable&&) = default;
  auto operator=(const Serializable&) -> Serializable& = default;
  auto operator=(Serializable&&) -> Serializable& = default;
  virtual ~Serializable() = default;
};

}  // namespace VIPRA::Modules