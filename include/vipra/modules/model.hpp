#pragma once

#include "vipra/macros/model.hpp"
#include "vipra/modules/goals.hpp"
#include "vipra/modules/pedestrians.hpp"
#include "vipra/random/random.hpp"

#include "vipra/types/state.hpp"
#include "vipra/types/time.hpp"

namespace VIPRA::Modules {
/**
 * @brief Base Model module
 * 
 */
class Model {
 public:
  virtual VIPRA_MODEL_TIMESTEP = 0;
  virtual VIPRA_MODEL_INIT_STEP = 0;

  Model() = default;
  Model(const Model&) = default;
  Model(Model&&) = default;
  auto operator=(const Model&) -> Model& = default;
  auto operator=(Model&&) -> Model& = default;
  virtual ~Model() = default;
};

}  // namespace VIPRA::Modules