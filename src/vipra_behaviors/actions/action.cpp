

#include <utility>

#include "vipra/vipra_behaviors/actions/action.hpp"
#include "vipra/vipra_behaviors/definitions/sim_pack.hpp"
#include "vipra/vipra_behaviors/targets/target.hpp"
#include "vipra/vipra_behaviors/targets/target_selector.hpp"
#include "vipra/vipra_behaviors/time/time.hpp"
#include "vipra/vipra_behaviors/util/timed_latch.hpp"
#include "vipra/vipra_behaviors/values/numeric_value.hpp"

namespace VIPRA::Behaviors {

/**
 * @brief Initializes the actions duration container
 * 
 * @param pack : simulation pack
 */
void Action::initialize(Simpack pack) {
  if (_condition) _condition->initialize(pack);
  if (_duration) _duration->resize(pack.get_pedset().getNumPedestrians());
}

/**
 * @brief Checks that the action conditions are met, if it is, each atom is run on the pedestrian
 * 
 * @param pedSet : pedestrian set module
 * @param obsSet : obstacle set module
 * @param goals : goals module
 * @param context : behavior context
 * @param pedIdx : index of pedestrian to affect
 * @param dT : simulation timestep size
 * @param state : state object to apply changes to
 */
void Action::perform_action(Simpack pack, const VIPRA::idxVec& peds, std::vector<bool> const& conditionMet,
                            std::vector<Target> const& targets) {
  std::for_each(_atoms.begin(), _atoms.end(), [&](Atom& atom) { atom(pack, peds, conditionMet, targets); });
}

/**
 * @brief Performs the action on the each pedestrian unconditionally
 * 
 * @param pack 
 * @param peds 
 * @param targets 
 */
void Action::perform_action(Simpack pack, VIPRA::idxVec& peds, std::vector<Target> const& targets) {
  std::vector<bool> conditionMet;
  std::for_each(_atoms.begin(), _atoms.end(), [&](Atom& atom) { atom(pack, peds, conditionMet, targets); });
}

/**
 * @brief Sets the condition for the action
 * 
 * @param cond : Condition object
 */
void Action::add_condition(Condition const& cond) { _condition = cond; }

/**
 * @brief Adds an atom to the action
 * 
 * @param atom : 
 */
void Action::add_atom(Atom const& atom) { _atoms.emplace_back(atom); }

/**
 * @brief Adds a time range for the action to take place
 * 
 * @param range
 */
void Action::add_duration(Behaviors::NumericValue const& dur) { _duration = TimedLatchCollection(dur); }

/**
 * @brief Sets the target selector for the action
 * 
 * @param target : target selector
 */
void Action::add_target(TargetSelector&& target) { _targets = target; }

}  // namespace VIPRA::Behaviors