

#include <randomization/random.hpp>
#include <selectors/selector_percent.hpp>

namespace VIPRA::Behaviors {
/**
 * @brief Selects (numpeds * ratio) pedestrians for the simulation
 * 
 * @param pedSet : 
 */
auto SelectorPercent::operator()(const VIPRA::idxVec& fullGroup, const VIPRA::idxVec& group,
                                 Simpack pack) const -> SelectorResult {
  auto groupPeds = group;

  auto count = static_cast<VIPRA::size>(std::floor(percentage * static_cast<float>(fullGroup.size())));

  bool starved = false;
  if (count > group.size()) {
    starved = true;
    count = group.size();
  }

  // spdlog::debug("Selector Percent: Selecting {} Pedestrians", count);

  std::shuffle(groupPeds.begin(), groupPeds.end(), pack.get_context().engine);
  groupPeds.resize(count);

  return {starved, groupPeds};
}
}  // namespace VIPRA::Behaviors