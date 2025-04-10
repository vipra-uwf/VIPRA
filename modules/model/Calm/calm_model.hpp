#pragma once

#include "vipra/modules/map.hpp"
#include "vipra/modules/model.hpp"

#include "vipra/macros/model.hpp"
#include "vipra/macros/module.hpp"
#include "vipra/macros/parameters.hpp"
#include "vipra/macros/performance.hpp"

#include "calm_collision.hpp"
#include "calm_model_types.hpp"
#include "vipra/types/float.hpp"

namespace Model {
class Calm : public VIPRA::Modules::Module<Calm>, public VIPRA::Modules::Model {
 public:
  VIPRA_MODULE_NAME("Calm")
  VIPRA_MODULE_TYPE(Model)

  VIPRA_REGISTER_PARAMS(VIPRA_PARAM("meanMass", _config.meanMass),
                        VIPRA_PARAM("massStdDev", _config.massStdDev),
                        VIPRA_PARAM("meanMass", _config.meanMass),
                        VIPRA_PARAM("massStdDev", _config.massStdDev),
                        VIPRA_PARAM("meanReactionTime", _config.meanReactionTime),
                        VIPRA_PARAM("reactionTimeStdDev", _config.reactionTimeStdDev),
                        VIPRA_PARAM("meanMaxSpeed", _config.meanMaxSpeed),
                        VIPRA_PARAM("maxSpeedStdDev", _config.maxSpeedStdDev),
                        VIPRA_PARAM("meanShoulderLen", _config.meanShoulderLen),
                        VIPRA_PARAM("shoulderLenStdDev", _config.shoulderLenStdDev),
                        VIPRA_PARAM("collisionRange", _config.collisionRange),
                        VIPRA_PARAM("friction", _config.frictionCoef))

  VIPRA_MODEL_RESET {}

  // NOLINTNEXTLINE(misc-unused-parameters)
  VIPRA_MODEL_INIT_STEP
  {
    _peds.resize(pedset.num_pedestrians());
    _collision.initialize(_config.collisionRange, pedset);

    _peds.masses = VIPRA::Random::make_distribution(
        std::normal_distribution<VIPRA::f_pnt>{_config.meanMass, _config.massStdDev},
        _peds.size(), engine);
    _peds.reactionTimes = VIPRA::Random::make_distribution<>(
        std::normal_distribution<VIPRA::f_pnt>{_config.meanReactionTime,
                                               _config.reactionTimeStdDev},
        _peds.size(), engine);
    _peds.maxSpeeds = VIPRA::Random::make_distribution<>(
        std::normal_distribution<VIPRA::f_pnt>{_config.meanMaxSpeed,
                                               _config.maxSpeedStdDev},
        _peds.size(), engine);
    _peds.shoulderLens = VIPRA::Random::make_distribution<>(
        std::normal_distribution<VIPRA::f_pnt>{_config.meanShoulderLen,
                                               _config.shoulderLenStdDev},
        _peds.size(), engine);
  }

  // NOLINTNEXTLINE(bugprone-easily-swappable-parameters, misc-unused-parameters)
  VIPRA_MODEL_TIMESTEP
  {
    calc_shoulders(pedset.all_coords(), goals.current_goals());
    calc_neighbors(pedset, goals, map);
    calc_betas();
    if ( timestep % 100 == 0 ) _collision.race_detection(pedset, goals, _peds);

    update_state(pedset, goals, state, deltaT);
  }

 private:
  CALM::ModelData          _peds;
  CALM::ConfigData         _config{};
  CALM::CollisionDetection _collision;

  void calc_neighbors(VIPRA::Modules::Pedestrians const& pedset,
                      VIPRA::Modules::Goals const& goals, VIPRA::Modules::Map const& map);
  void update_state(VIPRA::Modules::Pedestrians const& pedset,
                    VIPRA::Modules::Goals const& goals, VIPRA::State& state,
                    VIPRA::delta_t deltaT);
  auto is_path_blocked(VIPRA::idx pedIdx, VIPRA::f3d veloc, VIPRA::f_pnt maxDist,
                       VIPRA::Modules::Map const& map) -> VIPRA::f_pnt;

  void        calc_shoulders(VIPRA::f3dVec const&, VIPRA::f3dVec const&);
  static auto obj_spatial_test(const VIPRA::Geometry::Rectangle&, VIPRA::f3d,
                               VIPRA::f3d) -> bool;
  static auto obj_direction_test(VIPRA::f3d, VIPRA::f3d, VIPRA::f3d) -> bool;
  static auto is_ped_toward_goal(VIPRA::f3d, VIPRA::f3d, VIPRA::f3d) -> bool;

  auto rect_from_shoulders(VIPRA::idx, VIPRA::f3d,
                           VIPRA::f3d) -> VIPRA::Geometry::Rectangle;

  void                               calc_betas();
  VIPRA_INLINE static constexpr auto calc_beta(VIPRA::f_pnt distance) -> VIPRA::f_pnt
  {
    constexpr VIPRA::f_pnt VAL_A = -2.11;
    constexpr VIPRA::f_pnt VAL_B = 0.366;
    constexpr VIPRA::f_pnt VAL_C = 0.966;
    return (VAL_C - std::exp(VAL_A * (distance - VAL_B)));
  }
};
}  // namespace Model