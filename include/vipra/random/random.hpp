#pragma once

#include <cstdint>
#include <cstdlib>
#include <limits>

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
  explicit Engine(uint64_t seedVal) : _currVal(seedVal) { _currVal <<= SHIFT_NUM; }

  static constexpr uint64_t DEFAULT_SEED = 12345;

  /**
   * @brief Produces the next pseudo random number in the sequence, uses Lehmer's generator
   * 
   * @return uint64_t 
   */
  auto operator()() noexcept -> uint64_t {
    _currVal *= MULT_NUM;
    return _currVal >> SHIFT_NUM;
  }

  /**
   * @brief Restarts the number generator with a new seed
   * 
   * @param seedNum : 
   */
  void reseed(uint64_t seedNum) noexcept {
    _currVal = seedNum;
    _currVal <<= SHIFT_NUM;
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
  static inline constexpr auto max() noexcept -> uint64_t { return std::numeric_limits<uint64_t>::max(); }

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
}  // namespace VIPRA::Random
