#pragma once

#include "vipra/concepts/module.hpp"
#include "vipra/modules.hpp"

namespace VIPRA::Concepts {
template <typename field_t>
concept FieldModule = Module<field_t, Modules::Type::FIELD>;

class DummyField {
  VIPRA_MODULE_TYPE(FIELD);

 public:
  template <typename params_t>
  static void register_params() {}

  void setup(auto& params) {}
};

CHECK_MODULE(FieldModule, DummyField)
}  // namespace VIPRA::Concepts