#pragma once

#include <vector>

#include "model/Calm/calm_model_types.hpp"

#include "vipra/geometry/f3d.hpp"
#include "vipra/geometry/rectangle.hpp"
#include "vipra/modules/goals.hpp"
#include "vipra/modules/pedestrians.hpp"
#include "vipra/types/float.hpp"
#include "vipra/types/idx.hpp"

namespace CALM {

struct Data {
  std::vector<float>        betas;
  std::vector<float>        masses;
  std::vector<float>        desiredSpeeds;
  std::vector<float>        shoulders;
  std::vector<VIPRA::f_pnt> nearestNeighborDists;
};

class CollisionDetection {
 public:
  enum class Status { WON, LOST };

  void initialize(VIPRA::f_pnt range, VIPRA::Modules::Pedestrians const& pedset);
  void race_detection(VIPRA::Modules::Pedestrians const& pedset,
                      VIPRA::Modules::Goals const& goals, CALM::ModelData const& data);

  [[nodiscard]] auto status(VIPRA::idx pedIdx) -> Status;

 private:
  VIPRA::f_pnt                            _range;
  std::vector<Status>                     _statuses;
  std::vector<VIPRA::Geometry::Rectangle> _collisionRects;

  void calc_collision_shapes(VIPRA::Modules::Pedestrians const& /*pedset*/,
                             CALM::ModelData const& /*data*/);
  void calc_races(VIPRA::Modules::Pedestrians const& /*pedset*/,
                  VIPRA::Modules::Goals const& /*goals*/);
  auto calc_collisions(VIPRA::idx /*pedIdx*/,
                       VIPRA::Modules::Pedestrians const& /*pedset*/,
                       VIPRA::Modules::Goals const& /*goals*/) -> bool;
  auto calc_intersection_midpoint(VIPRA::idx /*index1*/,
                                  VIPRA::idx /*index2*/) -> VIPRA::f3d;
};
}  // namespace CALM