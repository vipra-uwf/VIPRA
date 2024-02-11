#pragma once

#include <string>
#include <utility>

#include "vipra/geometry/f3d.hpp"
#include "vipra/types/time.hpp"

namespace VIPRA::Behaviors {
class Duration {
 public:
  explicit Duration(VIPRA::f_pnt dur) : _range(dur, dur) {}
  explicit Duration(std::pair<VIPRA::f_pnt, VIPRA::f_pnt> rnd) : _range(std::move(rnd)) {}
  Duration(VIPRA::f_pnt start, VIPRA::f_pnt end) : _range(start, end) {}

  [[nodiscard]] static auto duration_str() -> std::string { return ""; }

 private:
  VIPRA::time_range_s _range;
};
}  // namespace VIPRA::Behaviors
