#pragma once

#include <algorithm>

#include "vipra/geometry/f3d.hpp"

#include "vipra/logging/logging.hpp"

#include "vipra/macros/goals.hpp"
#include "vipra/macros/performance.hpp"

#include "vipra/modules/map.hpp"
#include "vipra/modules/pedestrians.hpp"
#include "vipra/random/random.hpp"

#include "vipra/types/float.hpp"
#include "vipra/types/idx.hpp"
#include "vipra/types/size.hpp"
#include "vipra/types/time.hpp"

namespace VIPRA::Modules {
/**
 * @brief Goals module mixin
 * 
 */
class Goals {
 public:
  // FORWARD_REGISTER_PARAMS;

  virtual VIPRA_GOALS_INIT_STEP = 0;
  virtual VIPRA_GOALS_UPDATE_STEP = 0;
  virtual VIPRA_GOALS_NEXT_GOAL = 0;
  virtual VIPRA_GOALS_CHANGE_GOAL = 0;

  void initialize(VIPRA::Modules::Pedestrians const& pedset,
                  VIPRA::Modules::Map const& map, VIPRA::Random::Engine& engine)
  {
    assert(pedset.num_pedestrians() > 0);

    VIPRA::Log::debug("Initializing Goals");

    VIPRA::size const pedCnt = pedset.num_pedestrians();

    _currentGoals.resize(pedCnt);
    _endGoals.resize(pedCnt);
    _timeSinceLastGoal = std::vector<VIPRA::f_pnt>(pedCnt, 0);
    _met = std::vector<bool>(pedCnt, false);

    init_step(pedset, map, engine);

    assert(_timeSinceLastGoal.size() == pedCnt);
    assert(_currentGoals.size() == pedCnt);
    assert(_endGoals.size() == pedCnt);

    VIPRA::Log::debug("Goals Initialized");
  }

  VIPRA_PERF_FUNC void update(Modules::Pedestrians const& pedset,
                              Modules::Map const& map, VIPRA::delta_t deltaT)
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
          if ( next_goal(pedIdx, pedset, map, deltaT) ) _met[pedIdx] = true;
          _timeSinceLastGoal[pedIdx] = 0.0F;
        }
      }
    }

    update_step(pedset, map, deltaT);
  }

  void change_end_goal(VIPRA::idx pedIdx, VIPRA::f3d currPos,
                       VIPRA::f3d goalPos, VIPRA::Random::Engine& engine)
  {
    assert(pedIdx < _endGoals.size());
    return change_end_goal_impl(pedIdx, currPos, goalPos, engine);
  }

  [[nodiscard]] VIPRA_INLINE auto current_goals() const -> const VIPRA::f3dVec&
  {
    return _currentGoals;
  }

  [[nodiscard]] VIPRA_INLINE auto end_goals() const -> const VIPRA::f3dVec&
  {
    return _endGoals;
  }

  [[nodiscard]] VIPRA_INLINE auto current_goal(VIPRA::idx pedIdx) const
      -> VIPRA::f3d const&
  {
    return _currentGoals[pedIdx];
  }

  [[nodiscard]] VIPRA_INLINE auto end_goal(VIPRA::idx pedIdx) const
      -> VIPRA::f3d const&
  {
    return _endGoals[pedIdx];
  }

  [[nodiscard]] VIPRA_INLINE auto is_goal_met(VIPRA::idx pedIdx) const -> bool
  {
    return _met[pedIdx];
  }

  [[nodiscard]] VIPRA_INLINE auto is_sim_goal_met() const -> bool
  {
    return std::all_of(_met.begin(), _met.end(), [](bool met) { return met; });
  }

  [[nodiscard]] VIPRA_INLINE auto time_since_last_goal(VIPRA::idx pedIdx) const
      -> VIPRA::f_pnt
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
  VIPRA_INLINE void set_goal_met(VIPRA::idx pedIdx, bool met)
  {
    _met[pedIdx] = met;
  }
  VIPRA_INLINE void set_end_goal(VIPRA::idx pedIdx, VIPRA::f3d pos)
  {
    _endGoals[pedIdx] = pos;
  }
  VIPRA_INLINE void set_current_goal(VIPRA::idx pedIdx, VIPRA::f3d pos)
  {
    _currentGoals[pedIdx] = pos;
  }

 public:
  Goals() = default;
  Goals(const Goals&) = default;
  Goals(Goals&&) noexcept = default;
  auto operator=(const Goals&) -> Goals& = default;
  auto operator=(Goals&&) noexcept -> Goals& = default;
  virtual ~Goals() = default;
};
}  // namespace VIPRA::Modules
