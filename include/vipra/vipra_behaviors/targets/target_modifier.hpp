#ifndef VIPRA_BEHAVIORS_TARGET_MODIFIER_HPP
#define VIPRA_BEHAVIORS_TARGET_MODIFIER_HPP

#include "definitions/sim_pack.hpp"
#include "targets/target.hpp"

namespace BHVR {

/**
 * @brief Modifier function for targets, returns if the target matches a condition
 * 
 */
using Modifier = std::function<bool(Simpack, VIPRA::idx, VIPRA::idx)>;

/**
 * @brief Modifies who can be a target for an action, by distance, direction, etc.
 * 
 */
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
  [[nodiscard]] auto check(Simpack pack, VIPRA::idx self, VIPRA::idx target) const -> bool {
    return std::all_of(_checks.begin(), _checks.end(),
                       [&](const Modifier& modifier) { return modifier(pack, target, self); });
  }

  /**
   * @brief Adds another modifier check to the target modifier
   * 
   * @param check : check to add
   */
  void add_check(Modifier check) { _checks.emplace_back(check); }

 private:
  std::vector<Modifier> _checks;
};
}  // namespace BHVR

#endif