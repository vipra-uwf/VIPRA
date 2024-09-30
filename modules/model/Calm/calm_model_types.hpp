#pragma once

#include "vipra/geometry/line.hpp"
#include "vipra/types/float.hpp"
#include "vipra/types/size.hpp"

namespace CALM {
enum RaceStatus { NO_RACE, WAIT };

struct ConfigData {
  VIPRA::f_pnt meanMass;
  VIPRA::f_pnt massStdDev;
  VIPRA::f_pnt meanReactionTime;
  VIPRA::f_pnt reactionTimeStdDev;
  VIPRA::f_pnt meanMaxSpeed;
  VIPRA::f_pnt maxSpeedStdDev;
  VIPRA::f_pnt meanShoulderLen;
  VIPRA::f_pnt shoulderLenStdDev;
  VIPRA::size  randomSeed;
};

struct ModelData {
  std::vector<VIPRA::f_pnt>          masses;
  std::vector<VIPRA::f_pnt>          reactionTimes;
  std::vector<VIPRA::f_pnt>          maxSpeeds;
  std::vector<VIPRA::f_pnt>          shoulderLens;
  std::vector<VIPRA::f_pnt>          betas;
  std::vector<VIPRA::f_pnt>          nearestDists;
  std::vector<VIPRA::Geometry::Line> shoulders;

  [[nodiscard]] auto size() const noexcept -> size_t { return masses.size(); }

  void resize(size_t size)
  {
    masses.resize(size);
    reactionTimes.resize(size);
    maxSpeeds.resize(size);
    shoulderLens.resize(size);
    betas.resize(size);
    nearestDists.resize(size);
    shoulders.resize(size);
  }
};
}  // namespace CALM