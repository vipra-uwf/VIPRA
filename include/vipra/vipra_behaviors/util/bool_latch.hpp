#pragma once

#include "vipra/vipra_behaviors/util/class_types.hpp"

namespace VIPRA::Behaviors {
/**
   * @brief Literally a boolean with extra syntax
   * 
   */
class Latch {
  DEFAULT_CONSTRUCTIBLE(Latch)
  COPYABLE(Latch)
  MOVEABLE(Latch)
 public:
  inline void latch() { _set = true; }

  inline void unlatch() { _set = false; }

  [[nodiscard]] inline explicit operator bool() const { return _set; }

 private:
  bool _set = false;
};
}  // namespace VIPRA::Behaviors
