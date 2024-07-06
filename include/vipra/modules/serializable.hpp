#pragma once

#include <string>

#include "vipra/modules/module.hpp"

namespace VIPRA::Modules {

template <typename module_t>
class Serializable : public Module<Serializable<module_t>> {
 public:
  [[nodiscard]] auto serialize() -> std::string { return this->self().to_string(); }

  auto parse(std::string const& data) -> bool { return this->self().parse_impl(data); }
};

}  // namespace VIPRA::Modules