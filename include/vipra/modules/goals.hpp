#pragma once

#include <algorithm>

#include "vipra/geometry/f3d.hpp"

#include "vipra/logging/logging.hpp"
#include "vipra/macros/performance.hpp"
#include "vipra/modules/util.hpp"
#include "vipra/random/random.hpp"
#include "vipra/types/float.hpp"
#include "vipra/types/idx.hpp"
#include "vipra/types/size.hpp"
#include "vipra/types/time.hpp"

#include "vipra/util/crtp.hpp"

namespace VIPRA::Modules {
/**
 * @brief Goals module mixin
 * 
 */
template <typename module_t>
class Goals : public Util::CRTP<Goals<module_t>> {
  using Util::CRTP<Goals<module_t>>::self;

 public:
  FORWARD_REGISTER_PARAMS;

  template <typename pedset_t, typename map_t>
  void initialize(pedset_t const& pedset, map_t const& map, VIPRA::Random::Engine& engine)
  {
    assert(pedset.num_pedestrians() > 0);

    VIPRA::Log::debug("Initializing Goals");

    VIPRA::size const pedCnt = pedset.num_pedestrians();

    _currentGoals.resize(pedCnt);
    _endGoals.resize(pedCnt);
    _timeSinceLastGoal = std::vector<VIPRA::f_pnt>(pedCnt, 0);
    _met = std::vector<bool>(pedCnt, false);

    self().init_step(pedset, map, engine);

    assert(_timeSinceLastGoal.size() == pedCnt);
    assert(_currentGoals.size() == pedCnt);
    assert(_endGoals.size() == pedCnt);

    VIPRA::Log::debug("Goals Initialized");
  }

  template <typename pedset_t, typename map_t>
  VIPRA_PERF_FUNC void update(pedset_t const& pedset, map_t const& map, VIPRA::delta_t deltaT)
  {
    assert(pedset.num_pedestrians() > 0);

    for ( VIPRA::idx pedIdx = 0; pedIdx < pedset.num_pedestrians(); ++pedIdx ) {
      if ( _met[pedIdx] ) continue;

      auto const pos = pedset.ped_coords(pedIdx);
      _timeSinceLastGoal[pedIdx] += deltaT;

      if ( pos.distance_to(end_goal(pedIdx)) < MIN_GOAL_DIST ) {
        _met[pedIdx] = true;
      }
      else {
        if ( pos.distance_to(current_goal(pedIdx)) < MIN_GOAL_DIST ) {
          if ( self().next_goal(pedIdx, pedset, map, deltaT) ) _met[pedIdx] = true;
          _timeSinceLastGoal[pedIdx] = 0.0F;
        }
      }
    }

    self().update_step(pedset, map, deltaT);
  }

  void change_end_goal(VIPRA::idx pedIdx, VIPRA::f3d currPos, VIPRA::f3d goalPos,
                       VIPRA::Random::Engine& engine)
  {
    assert(pedIdx < _endGoals.size());
    return self().change_end_goal_impl(pedIdx, currPos, goalPos, engine);
  }

  [[nodiscard]] VIPRA_INLINE auto current_goals() const -> const VIPRA::f3dVec& { return _currentGoals; }

  [[nodiscard]] VIPRA_INLINE auto end_goals() const -> const VIPRA::f3dVec& { return _endGoals; }

  [[nodiscard]] VIPRA_INLINE auto current_goal(VIPRA::idx pedIdx) const -> VIPRA::f3d const&
  {
    return _currentGoals[pedIdx];
  }

  [[nodiscard]] VIPRA_INLINE auto end_goal(VIPRA::idx pedIdx) const -> VIPRA::f3d const&
  {
    return _endGoals[pedIdx];
  }

  [[nodiscard]] VIPRA_INLINE auto is_goal_met(VIPRA::idx pedIdx) const -> bool { return _met[pedIdx]; }

  [[nodiscard]] VIPRA_INLINE auto is_sim_goal_met() const -> bool
  {
    return std::all_of(_met.begin(), _met.end(), [](bool met) { return met; });
  }

  [[nodiscard]] VIPRA_INLINE auto time_since_last_goal(VIPRA::idx pedIdx) const -> VIPRA::f_pnt
  {
    assert(_timeSinceLastGoal.size() > pedIdx);
    return _timeSinceLastGoal[pedIdx];
  }

 private:
  VIPRA::f3dVec             _currentGoals;
  VIPRA::f3dVec             _endGoals;
  std::vector<VIPRA::f_pnt> _timeSinceLastGoal;

  std::vector<bool> _met;

  static constexpr VIPRA::f_pnt MIN_GOAL_DIST = 0.15;

 protected:
  VIPRA_INLINE void set_goal_met(VIPRA::idx pedIdx, bool met) { _met[pedIdx] = met; }
  VIPRA_INLINE void set_end_goal(VIPRA::idx pedIdx, VIPRA::f3d pos) { _endGoals[pedIdx] = pos; }
  VIPRA_INLINE void set_current_goal(VIPRA::idx pedIdx, VIPRA::f3d pos) { _currentGoals[pedIdx] = pos; }
};
}  // namespace VIPRA::Modules
