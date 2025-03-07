#pragma once

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <limits>
#include <vector>
#include "vipra/concepts/numeric.hpp"
#include "vipra/types/float.hpp"
#include "vipra/types/size.hpp"

namespace VIPRA::Random {

/**
 * @brief Psuedo Random number engine
 * 
 */
// NOLINTNEXTLINE (rolland) Default Naming Convention looks ugly
class Engine {
  using seed = __uint128_t;

 public:
  using result_type = uint64_t;

  Engine() : _currVal(DEFAULT_SEED) {}
  explicit Engine(uint64_t seedVal) : _currVal(seedVal)
  {
    assert(seedVal != 0);

    _currVal <<= SHIFT_NUM;
  }

  static constexpr uint64_t DEFAULT_SEED = 12345;

  /**
   * @brief Produces the next pseudo random number in the sequence, uses Lehmer's generator
   * 
   * @return uint64_t 
   */
  auto operator()() noexcept -> uint64_t
  {
    assert(_currVal != 0);

    _currVal *= MULT_NUM;
    return _currVal >> SHIFT_NUM;
  }

  /**
   * @brief Restarts the number generator with a new seed
   * 
   * @param seedNum : 
   */
  void reseed(uint64_t seedNum) noexcept
  {
    _currVal = seedNum;
    _currVal <<= SHIFT_NUM;

    assert(_currVal != 0);
  }

  /**
   * @brief Returns the minimum number possible
   * 
   * @return constexpr uint64_t 
   */
  static inline constexpr auto min() noexcept -> uint64_t { return 0; }

  /**
   * @brief Returns the maximum number possible
   * 
   * @return constexpr uint64_t 
   */
  static inline constexpr auto max() noexcept -> uint64_t
  {
    return std::numeric_limits<uint64_t>::max();
  }

 private:
  seed                      _currVal;
  static constexpr uint64_t MULT_NUM = 0xda942042e4dd58b5;
  static constexpr uint64_t SHIFT_NUM = 64;

 public:
  ~Engine() = default;
  Engine(const Engine&) = default;
  Engine(Engine&&) noexcept = default;
  auto operator=(const Engine&) -> Engine& = default;
  auto operator=(Engine&&) noexcept -> Engine& = default;
};

/**
 * @brief Creates a vector of values that follows the provided distribution
 * 
 * @tparam dist_t 
 * @tparam data_t 
 * @param distr 
 * @param count 
 * @param engine 
 * @return std::vector<data_t> 
 */
template <typename dist_t, Concepts::Numeric data_t = VIPRA::f_pnt>
inline auto make_distribution(dist_t&& distr, VIPRA::size count,
                              VIPRA::Random::Engine& engine) -> std::vector<data_t>
{
  std::vector<data_t> ret(count);

  std::transform(ret.cbegin(), ret.cend(), ret.begin(), [&](data_t) {
    data_t val = distr(engine);
    return val;
  });

  return ret;
}

}  // namespace VIPRA::Random
