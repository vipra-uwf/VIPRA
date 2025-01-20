#pragma once

#include <optional>
#include <tuple>
#include <vector>

#include "badl/definitions/time.hpp"

namespace BADL {
template <typename... memory_ts>
class Memory {
 public:
  template <typename memory_t>
  struct Instance {
    memory_t   info;
    BADL::time timePercieved;

    constexpr auto operator<(BADL::time const& checkTime) const noexcept
    {
      return timePercieved < checkTime;
    }
  };

  template <typename memory_t>
  using Container = std::vector<Instance<memory_t>>;

  template <typename memory_t>
  void clear()
  {
    std::get<std::vector<Instance<memory_t>>>(_memories).clear();
  }

  template <typename memory_t>
  void add_memory(memory_t&& memory, BADL::time timePercieved);

  template <typename memory_t>
  [[nodiscard]] auto has_memory() const noexcept -> bool;
  template <typename memory_t>
  [[nodiscard]] auto has_memory_since(BADL::time time) const noexcept -> bool;

  template <typename memory_t>
  [[nodiscard]] auto last_memory() const -> Instance<memory_t> const&;
  template <typename memory_t>
  [[nodiscard]] auto first_memory() const -> Instance<memory_t> const&;
  template <typename memory_t>
  [[nodiscard]] auto next_memory_after(BADL::time time) const
      -> std::optional<Instance<memory_t> const&>;

 private:
  std::tuple<std::vector<Instance<memory_ts>>...> _memories;

  template <typename memory_t>
  [[nodiscard]] constexpr auto get() noexcept
      -> std::vector<Instance<std::remove_cvref_t<memory_t>>>&;
  template <typename memory_t>
  [[nodiscard]] constexpr auto get() const noexcept
      -> std::vector<Instance<std::remove_cvref_t<memory_t>>> const&;
};
}  // namespace BADL

#define BADL_MEMORY_MAIN_HPP

#include "./_detail/memory_detail.hpp"

#undef BADL_MEMORY_MAIN_HPP