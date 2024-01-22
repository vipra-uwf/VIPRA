#pragma once

namespace VIPRA {
struct Parameter {
  enum class Type { REQUIRED, OPTIONAL };

  Type type;
};
}  // namespace VIPRA