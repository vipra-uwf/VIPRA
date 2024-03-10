#pragma once

#include <algorithm>
#include <random>

#include <string>
#include <vector>

#include "vipra/concepts/numeric.hpp"

#include "vipra/random/random.hpp"
#include "vipra/types/float.hpp"
#include "vipra/types/size.hpp"

namespace VIPRA::Random {

/**
 * @brief produces a uniform random distribution
 * 
 * @tparam data_t : 
 */
template <Concepts::Numeric data_t = VIPRA::f_pnt>
// NOLINTNEXTLINE (rolland) sticking to familiar naming convention for std::uniform_real_distribution
class uniform_distribution {
 public:
  constexpr uniform_distribution(data_t min, data_t max) : _range(min, max) {}
  constexpr explicit uniform_distribution(std::pair<data_t, data_t> rangeVals)
      : _range(std::move(rangeVals)) {}

  constexpr auto operator()(Random::Engine& engine) -> data_t {
    auto val = engine();
    if ((_range.second - _range.first) == 0) return _range.first;
    return static_cast<data_t>(_range.first + static_cast<double>(val) /
                                                  (static_cast<double>(std::numeric_limits<uint64_t>::max()) /
                                                   static_cast<double>(_range.second - _range.first)));
  }

 private:
  std::pair<data_t, data_t> _range{};
};

/**
 * @brief Normal Distribution created using the Marsaglia method
 * @ref https://en.wikipedia.org/wiki/Marsaglia_polar_method
 */
template <Concepts::Numeric data_t = VIPRA::f_pnt>
// NOLINTNEXTLINE (rolland) sticking to familiar naming convention for std::normal_distribution
class normal_distribution {
  // TODO(rolland): this is rather slow
 public:
  constexpr normal_distribution(data_t mean, data_t stdDev) : _mean(mean), _stdDev(stdDev) {}

  constexpr auto operator()(Random::Engine& engine) -> data_t {
    double resultX = 0;
    double resultY = 0;
    double result = 1.0;

    if (_hasSpare) {
      _hasSpare = false;
      return static_cast<data_t>(_mean + _stdDev * _spare);
    }

    while (result >= 1.0 || result == 0.0) {
      resultX = ((static_cast<double>(engine()) / static_cast<double>(Random::Engine::max())) * TWO) - 1.0;
      resultY = ((static_cast<double>(engine()) / static_cast<double>(Random::Engine::max())) * TWO) - 1.0;
      result = resultX * resultX + resultY * resultY;
    };

    result = std::sqrt(-TWO * std::log(result) / result);
    _spare = resultY * result;
    _hasSpare = true;
    return static_cast<data_t>(_mean + _stdDev * resultX * result);
  }

 private:
  data_t _mean;
  data_t _stdDev;

  double _spare{0.0F};
  bool   _hasSpare = false;

  static constexpr VIPRA::f_pnt TWO = 2.0;
};

template <typename dist_t, Concepts::Numeric data_t = VIPRA::f_pnt>
inline auto make_distribution(dist_t&& distr, VIPRA::size count, VIPRA::Random::Engine& engine)
    -> std::vector<data_t> {
  std::vector<data_t> ret(count);

  std::transform(ret.cbegin(), ret.cend(), ret.begin(), [&](data_t) {
    data_t val = distr(engine);
    return val;
  });

  return ret;
}
}  // namespace VIPRA::Random
