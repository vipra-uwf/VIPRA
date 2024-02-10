#pragma once

#include <string>
#include <utility>

#include "vipra/geometry/f3d.hpp"
#include "vipra/types/time.hpp"

namespace VIPRA::Behaviors {
using RandomVal = std::pair<float, float>;
class Duration {
 public:
  explicit Duration(float dur) : _range(dur, dur) {}
  explicit Duration(RandomVal rnd) : _range(std::move(rnd)) {}
  Duration(float start, float end) : _range(start, end) {}

  [[nodiscard]] static auto duration_str() -> std::string { return ""; }

 private:
  VIPRA::time_range_s _range;
};
}  // namespace VIPRA::Behaviors
