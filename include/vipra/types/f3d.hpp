#pragma once

#include <cmath>
#include <limits>
#include <stdexcept>
#include <vector>

namespace VIPRA {
// NOLINTNEXTLINE (rolland) Keeping f3d from before format changes :  ignore(readability-identifier-naming)
struct f3d {
  float x, y, z;

  ~f3d() = default;
  constexpr explicit f3d() noexcept : x(0), y(0), z(0) {}
  constexpr explicit f3d(float valX) noexcept : x(valX), y(0), z(0) {}
  constexpr explicit f3d(float valX, float valY) noexcept : x(valX), y(valY), z(0) {}
  constexpr explicit f3d(float valX, float valY, float valZ) noexcept : x(valX), y(valY), z(valZ) {}
  constexpr f3d(const f3d& other) noexcept = default;
  constexpr f3d(f3d&& other) noexcept = default;
  constexpr auto operator=(const f3d& other) noexcept -> f3d& = default;
  constexpr auto operator=(f3d&& other) noexcept -> f3d& = default;

  template <typename data_t, class = typename std::enable_if<std::is_integral<data_t>::value>::type>
  inline constexpr auto operator[](data_t index) -> float& {
    switch (index) {
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

  template <typename data_t, class = typename std::enable_if<std::is_integral<data_t>::value>::type>
  inline constexpr auto operator[](data_t index) const -> float {
    switch (index) {
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

  template <typename data_t>
  inline constexpr auto operator*(data_t&& multiplier) const noexcept -> f3d {
    return f3d{x, y, z} *= std::forward<data_t>(multiplier);
  }
  template <typename data_t>
  inline constexpr auto operator*=(data_t&& multiplier) noexcept -> f3d& {
    x *= multiplier;
    y *= multiplier;
    z *= multiplier;
    return *this;
  }

  template <typename data_t>
  inline constexpr auto operator/(data_t&& multiplier) const noexcept -> f3d {
    return f3d{x, y, z} /= std::forward<data_t>(multiplier);
  }
  template <typename data_t>
  inline constexpr auto operator/=(data_t&& multiplier) noexcept -> f3d& {
    x /= multiplier;
    y /= multiplier;
    z /= multiplier;
    return *this;
  }

  inline constexpr auto operator-(const f3d& other) const noexcept -> f3d {
    return f3d{x - other.x, y - other.y, z - other.z};
  }
  inline constexpr auto operator-(f3d&& other) const noexcept -> f3d {
    return f3d{x - other.x, y - other.y, z - other.z};
  }
  inline constexpr auto operator+(const f3d& other) const noexcept -> f3d {
    return f3d{x + other.x, y + other.y, z + other.z};
  }
  inline constexpr auto operator+(f3d&& other) const noexcept -> f3d {
    return f3d{x + other.x, y + other.y, z + other.z};
  }

  inline constexpr auto operator+=(const f3d& other) noexcept -> f3d& {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
  }
  inline constexpr auto operator+=(f3d&& other) noexcept -> f3d& {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
  }

  inline constexpr auto operator-=(const f3d& other) noexcept -> f3d& {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
  }
  inline constexpr auto operator-=(f3d&& other) noexcept -> f3d& {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
  }

  [[nodiscard]] inline constexpr auto distance_to_sqrd(const f3d& other) const noexcept -> float {
    const float dX = other.x - x;
    const float dY = other.y - y;
    const float dZ = other.z - z;

    return (dX * dX) + (dY * dY) + (dZ * dZ);
  }

  [[nodiscard]] inline constexpr auto distance_to(const f3d& other) const -> float {
    const float dX = other.x - x;
    const float dY = other.y - y;
    const float dZ = other.z - z;

    return std::sqrt((dX * dX) + (dY * dY) + (dZ * dZ));
  }

  inline constexpr auto distance_to(f3d&& other) const -> float {
    const float dX = other.x - x;
    const float dY = other.y - y;
    const float dZ = other.z - z;

    return std::sqrt((dX * dX) + (dY * dY) + (dZ * dZ));
  }

  inline constexpr auto operator==(const f3d& other) const noexcept -> bool {
    return (x == other.x && y == other.y && z == other.z);
  }
  inline constexpr auto operator==(f3d&& other) const noexcept -> bool {
    return (x == other.x && y == other.y && z == other.z);
  }

  inline constexpr auto operator!=(const f3d& other) const noexcept -> bool {
    return (x != other.x || y != other.y || z != other.z);
  }
  inline constexpr auto operator!=(f3d&& other) const noexcept -> bool {
    return (x != other.x || y != other.y || z != other.z);
  }

  inline constexpr auto operator-() const noexcept -> f3d { return f3d{-x, -y, -z}; }

  /**
   * @brief Returns the unit vector in the direction of the f3d
   * 
   * @return constexpr f3d 
   */
  [[nodiscard]] inline constexpr auto unit() const noexcept -> f3d {
    if (x == 0 && y == 0 && z == 0) {
      return f3d{0, 0, 0};
    }
    return f3d{x, y, z} / magnitude();
  }

  /**
   * @brief Returns vector magnitude^2
   * 
   * @return constexpr float 
   */
  [[nodiscard]] inline constexpr auto magnitude_sqrd() const noexcept -> float {
    return (x * x) + (y * y) + (z * z);
  }

  /**
   * @brief Returns the vectors magnitude
   * 
   * @return constexpr float 
   */
  [[nodiscard]] inline constexpr auto magnitude() const -> float {
    return std::sqrt((x * x) + (y * y) + (z * z));
  }

  /**
   * @brief Returns the dot product between two f3ds
   * 
   * @param other : 
   * @return constexpr float 
   */
  [[nodiscard]] inline constexpr auto dot(const f3d& other) const noexcept -> float {
    return (x * other.x) + (y * other.y) + (z * other.z);
  }
};

template <typename data_t>
inline constexpr auto operator*(data_t&& multiplier, const f3d& other) noexcept -> f3d {
  return f3d{other.x * multiplier, other.y * multiplier, other.z * multiplier};
}

using f3dVec = std::vector<f3d>;

inline constexpr const f3d _emptyf3d_ =  // NOLINT
    VIPRA::f3d{std::numeric_limits<float>::max(), std::numeric_limits<float>::max(),
               std::numeric_limits<float>::max()};

extern const f3dVec emptyf3d_vec;  // NOLINT
}  // namespace VIPRA