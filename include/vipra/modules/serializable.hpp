#pragma once

#include <string>

#include "vipra/modules/module.hpp"
#include "vipra/util/crtp.hpp"

namespace VIPRA::Modules {

template <typename module_t>
class Serializable : public Util::CRTP<Serializable<module_t>> {
  using Util::CRTP<Serializable<module_t>>::self;

 public:
  [[nodiscard]] auto serialize() -> std::string { return self().to_string(); }

  auto parse(std::string const& data) -> bool { return self().parse_impl(data); }
};

}  // namespace VIPRA::Modules