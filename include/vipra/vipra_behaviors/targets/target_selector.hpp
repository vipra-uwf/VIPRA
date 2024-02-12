#pragma once

#include <optional>

#include "vipra/vipra_behaviors/definitions/sim_pack.hpp"
#include "vipra/vipra_behaviors/targets/target.hpp"
#include "vipra/vipra_behaviors/targets/target_selectors/target_self.hpp"

namespace VIPRA::Behaviors {

/**
 * @brief Used to grab a target for either conditions or actions
 * 
 */
template <typename targetfunc_t>
class TargetSelector {
  DEFAULT_CONSTRUCTIBLE(TargetSelector)
  COPYABLE(TargetSelector)
  MOVEABLE(TargetSelector)

 public:
  explicit TargetSelector(targetfunc_t&& func) : _select(func) {}

  void get_targets(auto pack, const VIPRA::idxVec& peds, std::vector<Target>& targets) {
    if (!_select) {
      std::for_each(peds.begin(), peds.end(), [&](auto idx) {
        targets[idx] = Target{TargetType::PEDESTRIAN, idx};
      });
      return;
    }

    std::for_each(peds.begin(), peds.end(), [&](auto idx) {
      targets[idx] = _select.value()(pack, Self{Target{TargetType::PEDESTRIAN, idx}});
    });
  }

 private:
  std::optional<targetfunc_t> _select;
};
}  // namespace VIPRA::Behaviors
