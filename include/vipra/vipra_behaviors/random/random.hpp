#pragma once

#include <cstdint>
#include <limits>

#include "vipra/geometry/f3d.hpp"
#include "vipra/random/distributions.hpp"
#include "vipra/random/random.hpp"
#include "vipra/types/idx.hpp"

#include "vipra/vipra_behaviors/definitions/dsl_types.hpp"

namespace VIPRA::Behaviors {

struct Min {
  float val;
};
struct Max {
  float val;
};

class DRNG {
 public:
  /**
  * @brief Gets the random float between min and max that is assigned to the pedestrian at pedIdx
  * 
  */
  [[nodiscard]] static inline auto ped_random_float(Behaviors::seed seed, VIPRA::idx pedIdx, Min min, Max max)
      -> float {
    VIPRA::uniform_distribution<float> distr{min.val, max.val};
    return distr(get_engine(Behaviors::seed{seed + (pedIdx * PED_MULT_VAL)}));
  }

 private:
  static constexpr uint64_t PED_MULT_VAL{10037};

  static inline auto get_engine(Behaviors::seed seed) -> VIPRA::Random::Engine& {
    static VIPRA::Random::Engine gen{};
    gen.reseed(seed);
    return gen;
  }
};
}  // namespace VIPRA::Behaviors
