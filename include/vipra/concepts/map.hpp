#pragma once

#include "vipra/concepts/module.hpp"
#include "vipra/modules.hpp"

namespace VIPRA::Concepts {

template <typename map_t>
concept MapModule = Module<map_t, Modules::Type::MAP>;

class DummyMap {
  VIPRA_MODULE_TYPE(MAP);

 public:
  template <typename params_t>
  static void register_params() {}

  void setup(auto& params) {}
};

CHECK_MODULE(MapModule, DummyMap);
}  // namespace VIPRA::Concepts