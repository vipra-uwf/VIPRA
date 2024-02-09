#ifndef VIPRA_BEHAVIORS_RANDOM_HPP
#define VIPRA_BEHAVIORS_RANDOM_HPP

#include <cstdint>
#include <limits>

#include "definitions/dsl_types.hpp"
#include "vipra/geometry/f3d.hpp"

#include "distributions/distributions.hpp"
#include "randomization/random.hpp"

namespace BHVR {

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
  [[nodiscard]] static inline auto ped_random_float(BHVR::seed seed, VIPRA::idx pedIdx, Min min, Max max)
      -> float {
    VIPRA::uniform_distribution<float> distr{min.val, max.val};
    return distr(get_engine(BHVR::seed{seed + (pedIdx * PED_MULT_VAL)}));
  }

 private:
  static constexpr uint64_t PED_MULT_VAL{10037};

  static inline auto get_engine(BHVR::seed seed) -> VIPRA::pRNG_Engine& {
    static VIPRA::pRNG_Engine gen{};
    gen.reseed(seed);
    return gen;
  }
};
}  // namespace BHVR

#endif