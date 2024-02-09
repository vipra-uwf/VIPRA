#ifndef VIPRA_BEHAVIORS_TARGET_SELECTOR_HPP
#define VIPRA_BEHAVIORS_TARGET_SELECTOR_HPP

#include <definitions/type_definitions.hpp>
#include <optional>
#include "definitions/sim_pack.hpp"
#include "target.hpp"
#include "targets/target_selectors/target_self.hpp"

namespace BHVR {

/**
 * @brief Used to grab a target for either conditions or actions
 * 
 */
class TargetSelector {
  DEFAULT_CONSTRUCTIBLE(TargetSelector)
  COPYABLE(TargetSelector)
  MOVEABLE(TargetSelector)

 public:
  explicit TargetSelector(TargetFunc&& func) : _select(func) {}
  void get_targets(Simpack pack, const VIPRA::idxVec& peds, std::vector<Target>& targets) {
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
  std::optional<TargetFunc> _select;
};
}  // namespace BHVR

#endif