#pragma once

#include <map>
#include <optional>
#include <vector>

#include "badl/components/component.hpp"
#include "badl/definitions/time.hpp"

namespace BADL {
class Memory : public BADL::Component {
 public:
  auto get_condition_grammar()
      -> std::vector<std::pair<std::string_view, ConditionFunc>> override
  {
    return {
        {"has memory %str", &has_memory_condition},
    };
  };

  [[nodiscard]] static auto has_memory_condition(
      BADL::Agent const& agent, BADL::ProgramInterface const& /*unused*/,
      BADL::Environment<VIPRA::Sound, VIPRA::Sight> const& /*unused*/,
      BADL::ComponentParams const& params) noexcept -> bool;

  struct MemoryInstance {
    size_t     id;
    BADL::time timePercieved;

    constexpr auto operator<(BADL::time const& checkTime) const noexcept
    {
      return timePercieved < checkTime;
    }
  };

  void clear() { _memories.clear(); }

  void add_memory(size_t memory, BADL::time timePercieved);

  [[nodiscard]] auto has_memory(size_t memoryId) const noexcept -> bool;
  [[nodiscard]] auto has_memory_since(size_t     memoryId,
                                      BADL::time time) const noexcept -> bool;

  [[nodiscard]] auto last_memory(size_t memoryId) const
      -> MemoryInstance const&;
  [[nodiscard]] auto first_memory(size_t memoryId) const
      -> MemoryInstance const&;
  [[nodiscard]] auto next_memory_after(size_t memoryId, BADL::time time) const
      -> std::optional<MemoryInstance>;

 private:
  std::map<size_t, std::vector<MemoryInstance>> _memories;

  [[nodiscard]] auto get(size_t memoryId) noexcept
      -> std::vector<MemoryInstance>&;
  [[nodiscard]] auto get(size_t memoryId) const noexcept
      -> std::vector<MemoryInstance> const&;
};
}  // namespace BADL