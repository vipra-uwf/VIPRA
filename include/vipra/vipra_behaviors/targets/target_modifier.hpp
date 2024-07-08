#pragma once

#include "vipra/vipra_behaviors/definitions/sim_pack.hpp"
#include "vipra/vipra_behaviors/targets/target.hpp"

namespace VIPRA::Behaviors {

/**
 * @brief Modifier function for targets, returns if the target matches a condition
 * 
 */
template <typename pack_t>
using Modifier = std::function<bool(pack_t, VIPRA::idx, VIPRA::idx)>;

/**
 * @brief Modifies who can be a target for an action, by distance, direction, etc.
 * 
 */
template <typename modifier_t>
class TargetModifier {
  DEFAULT_CONSTRUCTIBLE(TargetModifier)
  COPYABLE(TargetModifier)
  MOVEABLE(TargetModifier)

 public:
  /**
   * @brief Checks that a pedestrian passes all modifier checks
   * 
   * @param pack : simulation pack
   * @param target : target to check
   * @param self : pedestrian looking for target
   * @return true : if target passes check
   * @return false : if target does NOT pass check
   */
  [[nodiscard]] auto check(auto pack, VIPRA::idx self, VIPRA::idx target) const -> bool
  {
    return std::all_of(_checks.begin(), _checks.end(),
                       [&](modifier_t const& modifier) { return modifier(pack, target, self); });
  }

  /**
   * @brief Adds another modifier check to the target modifier
   * 
   * @param check : check to add
   */
  void add_check(modifier_t&& check) { _checks.emplace_back(check); }

 private:
  std::vector<modifier_t> _checks;
};
}  // namespace VIPRA::Behaviors
