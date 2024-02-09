#ifndef VIPRA_BEHAVIORS_CLASS_TYPES_HPP
#define VIPRA_BEHAVIORS_CLASS_TYPES_HPP

namespace BHVR {
#define DEFAULT_CONSTRUCTIBLE(T) \
 public:                         \
  T() = default;                 \
  ~T() = default;

#define COPYABLE(T)      \
 public:                 \
  T(const T&) = default; \
  auto operator=(const T&)->T& = default;

#define MOVEABLE(T)          \
 public:                     \
  T(T&&) noexcept = default; \
  auto operator=(T&&) noexcept->T& = default;

#define NON_DEFAULT_CONSTRUCTIBLE(T) \
 public:                             \
  T() = delete;                      \
  ~T() = default;

#define NON_MOVEABLE(T) \
 public:                \
  T(T&&) = delete;      \
  auto operator=(T&&)->T& = delete;

#define NON_COPYABLE(T) \
 public:                \
  T(const T&) = delete; \
  auto operator=(const T&)->T& = delete;

#define SINGLETON(T) \
 public:             \
  T() = delete;      \
  ~T() = delete;     \
  NON_COPYABLE(T)    \
  NON_MOVABLE(T);

#define VIRTUAL(T) \
 public:           \
  T() = default;   \
  virtual ~T() = default;
}  // namespace BHVR

#endif