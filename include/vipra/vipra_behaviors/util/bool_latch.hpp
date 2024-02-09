#ifndef VIPRA_BEHAVIORS_BOOL_LATCH_HPP
#define VIPRA_BEHAVIORS_BOOL_LATCH_HPP

#include "util/class_types.hpp"
#include "vipra/geometry/f3d.hpp"

namespace BHVR {
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
}  // namespace BHVR

#endif