#pragma once

#include <cassert>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <vector>

#include "vipra/concepts/numeric.hpp"
#include "vipra/macros/performance.hpp"
#include "vipra/random/distributions.hpp"
#include "vipra/random/random.hpp"
#include "vipra/types/float.hpp"

#define F3D_FUNC [[nodiscard]] __attribute__((always_inline)) constexpr
#define F3D_FUNC_W_DISCARD __attribute__((always_inline)) constexpr

// TODO(rolland): do we need a 3d point class?
//                  - we can use a 2d point and in the case of multiple stories we can use a 3d point?
//                  - maybe an f2d with a small integer for the story?

namespace VIPRA {
// NOLINTNEXTLINE (rolland) Keeping f3d from before format changes :  ignore(readability-identifier-naming)
struct f3d {
  VIPRA::f_pnt x, y, z;

  ~f3d() = default;
  F3D_FUNC explicit f3d() noexcept : x(0), y(0), z(0) {}
  F3D_FUNC explicit f3d(VIPRA::f_pnt valX) noexcept : x(valX), y(0), z(0) {}
  F3D_FUNC explicit f3d(VIPRA::f_pnt valX, VIPRA::f_pnt valY) noexcept : x(valX), y(valY), z(0) {}
  F3D_FUNC explicit f3d(VIPRA::f_pnt valX, VIPRA::f_pnt valY, VIPRA::f_pnt valZ) noexcept
      : x(valX), y(valY), z(valZ)
  {
  }
  F3D_FUNC                f3d(f3d const& other) noexcept = default;
  F3D_FUNC                f3d(f3d&& other) noexcept = default;
  F3D_FUNC_W_DISCARD auto operator=(f3d const& other) noexcept -> f3d& = default;
  F3D_FUNC_W_DISCARD auto operator=(f3d&& other) noexcept -> f3d& = default;

  F3D_FUNC static auto random(VIPRA::f_pnt magnitude, VIPRA::Random::Engine& engine) -> VIPRA::f3d
  {
    Random::uniform_distribution<VIPRA::f_pnt> dist{-1.0, 1.0};
    VIPRA::f3d                                 retVal{dist(engine), dist(engine)};
    return retVal.unit() * magnitude;
  }

  template <Concepts::Numeric data_t>
  F3D_FUNC auto operator[](data_t index) -> VIPRA::f_pnt&
  {
    switch ( index ) {
      case 0:
        return x;
      case 1:
        return y;
      case 2:
        return z;
      case 'x':
        return x;
      case 'y':
        return y;
      case 'z':
        return z;
      default:
        throw std::out_of_range("Attempt to access invalid index on VIPRA::f3d");
    }
  }

  template <Concepts::Numeric data_t>
  F3D_FUNC auto operator[](data_t index) const -> VIPRA::f_pnt
  {
    switch ( index ) {
      case 0:
        return x;
      case 1:
        return y;
      case 2:
        return z;
      case 'x':
        return x;
      case 'y':
        return y;
      case 'z':
        return z;
      default:
        throw std::out_of_range("Attempt to access invalid index on VIPRA::f3d");
    }
  }

  template <Concepts::Numeric data_t>
  F3D_FUNC auto operator*(data_t&& multiplier) const noexcept -> f3d
  {
    return f3d{x, y, z} *= std::forward<data_t>(multiplier);
  }
  template <Concepts::Numeric data_t>
  F3D_FUNC_W_DISCARD auto operator*=(data_t&& multiplier) noexcept -> f3d&
  {
    x *= multiplier;
    y *= multiplier;
    z *= multiplier;
    return *this;
  }

  template <Concepts::Numeric data_t>
  F3D_FUNC auto operator/(data_t&& multiplier) const noexcept -> f3d
  {
    assert(multiplier != 0);

    return f3d{x, y, z} /= std::forward<data_t>(multiplier);
  }
  template <Concepts::Numeric data_t>
  F3D_FUNC_W_DISCARD auto operator/=(data_t&& multiplier) noexcept -> f3d&
  {
    assert(multiplier != 0);

    x /= multiplier;
    y /= multiplier;
    z /= multiplier;
    return *this;
  }

  F3D_FUNC auto operator-(f3d const& other) const noexcept -> f3d
  {
    return f3d{x - other.x, y - other.y, z - other.z};
  }
  F3D_FUNC auto operator-(f3d&& other) const noexcept -> f3d
  {
    return f3d{x - other.x, y - other.y, z - other.z};
  }
  F3D_FUNC auto operator+(f3d const& other) const noexcept -> f3d
  {
    return f3d{x + other.x, y + other.y, z + other.z};
  }
  F3D_FUNC auto operator+(f3d&& other) const noexcept -> f3d
  {
    return f3d{x + other.x, y + other.y, z + other.z};
  }

  F3D_FUNC_W_DISCARD auto operator+=(f3d const& other) noexcept -> f3d&
  {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
  }
  F3D_FUNC_W_DISCARD auto operator+=(f3d&& other) noexcept -> f3d&
  {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
  }

  F3D_FUNC_W_DISCARD auto operator-=(f3d const& other) noexcept -> f3d&
  {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
  }
  F3D_FUNC_W_DISCARD auto operator-=(f3d&& other) noexcept -> f3d&
  {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
  }

  F3D_FUNC auto distance_to_sqrd(f3d const& other) const noexcept -> VIPRA::f_pnt
  {
    const VIPRA::f_pnt deltaX = other.x - x;
    const VIPRA::f_pnt deltaY = other.y - y;
    const VIPRA::f_pnt deltaZ = other.z - z;

    return (deltaX * deltaX) + (deltaY * deltaY) + (deltaZ * deltaZ);
  }

  F3D_FUNC auto distance_to(f3d const& other) const -> VIPRA::f_pnt
  {
    const VIPRA::f_pnt deltaX = other.x - x;
    const VIPRA::f_pnt deltaY = other.y - y;
    const VIPRA::f_pnt deltaZ = other.z - z;

    return std::sqrt((deltaX * deltaX) + (deltaY * deltaY) + (deltaZ * deltaZ));
  }

  F3D_FUNC auto distance_to(f3d&& other) const -> VIPRA::f_pnt
  {
    const VIPRA::f_pnt deltaX = other.x - x;
    const VIPRA::f_pnt deltaY = other.y - y;
    const VIPRA::f_pnt deltaZ = other.z - z;

    return std::sqrt((deltaX * deltaX) + (deltaY * deltaY) + (deltaZ * deltaZ));
  }

  F3D_FUNC auto operator==(f3d const& other) const noexcept -> bool
  {
    return (x == other.x && y == other.y && z == other.z);
  }
  F3D_FUNC auto operator==(f3d&& other) const noexcept -> bool
  {
    return (x == other.x && y == other.y && z == other.z);
  }

  F3D_FUNC auto operator!=(f3d const& other) const noexcept -> bool
  {
    return (x != other.x || y != other.y || z != other.z);
  }
  F3D_FUNC auto operator!=(f3d&& other) const noexcept -> bool
  {
    return (x != other.x || y != other.y || z != other.z);
  }

  F3D_FUNC auto operator-() const noexcept -> f3d { return f3d{-x, -y, -z}; }

  /**
   * @brief Returns the unit vector in the direction of the f3d
   * 
   * @return constexpr f3d 
   */
  F3D_FUNC auto unit() const noexcept -> f3d
  {
    if ( x == 0 && y == 0 && z == 0 ) {
      return f3d{0, 0, 0};
    }
    return f3d{x, y, z} / mag();
  }

  /**
   * @brief Returns vector magnitude^2
   * 
   * @return constexpr VIPRA::f_pnt 
   */
  F3D_FUNC auto mag_sqrd() const noexcept -> VIPRA::f_pnt { return (x * x) + (y * y) + (z * z); }

  /**
   * @brief Returns the vectors magnitude
   * 
   * @return constexpr VIPRA::f_pnt 
   */
  F3D_FUNC auto mag() const -> VIPRA::f_pnt { return std::sqrt((x * x) + (y * y) + (z * z)); }

  /**
   * @brief Returns the dot product between two f3ds
   * 
   * @param other : 
   * @return constexpr VIPRA::f_pnt 
   */
  F3D_FUNC auto dot(f3d const& other) const noexcept -> VIPRA::f_pnt
  {
    return (x * other.x) + (y * other.y) + (z * other.z);
  }

  /**
   * @brief Returns the cross product between two f3ds
   * 
   * @param other 
   * @return f3d 
   */
  F3D_FUNC auto cross(f3d const& other) const noexcept -> f3d
  {
    return f3d{(y * other.z) - (z * other.y), (z * other.x) - (x * other.z), (x * other.y) - (y * other.x)};
  }

  /**
   * @brief Returns the string representation of the f3d
   * 
   * @return std::string 
   */
  [[nodiscard]] VIPRA_INLINE auto to_string() const -> std::string
  {
    return std::string{"("} + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
  }
};

template <Concepts::Numeric data_t>
F3D_FUNC auto operator*(data_t&& multiplier, f3d const& other) noexcept -> f3d
{
  return f3d{other.x * multiplier, other.y * multiplier, other.z * multiplier};
}

using f3dVec = std::vector<f3d>;

constexpr f3d _emptyf3d_ =  // NOLINT
    VIPRA::f3d{std::numeric_limits<VIPRA::f_pnt>::max(), std::numeric_limits<VIPRA::f_pnt>::max(),
               std::numeric_limits<VIPRA::f_pnt>::max()};

extern const f3dVec emptyf3d_vec;  // NOLINT
}  // namespace VIPRA