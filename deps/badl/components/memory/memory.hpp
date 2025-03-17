#pragma once

#include <map>
#include <optional>
#include <string>
#include <vector>

#include "badl/components/component.hpp"
#include "badl/definitions/time.hpp"

namespace BADL {
class Memory : public BADL::Component {
 public:
  [[nodiscard]] auto get_query_grammar() const
      -> std::vector<
          std::tuple<std::string, std::string_view, QueryFunc>> override
  {
    return {
        {"has_memory", "has memory %str", &has_memory_condition},
    };
  };

  [[nodiscard]] auto get_activation_grammar() const
      -> std::vector<
          std::tuple<std::string, std::string_view, ActivationFunc>> override
  {
    return {
        {"add_memory", "add memory %str", &add_memory_actuator},
    };
  };

  [[nodiscard]] static auto memory_map() -> std::map<std::string, size_t>&;
  [[nodiscard]] static auto memory_id(std::string const&) -> size_t;

  [[nodiscard]] static auto has_memory_condition(
      BADL::Agent const& agent, BADL::ProgramInterface const& /*unused*/,
      BADL::Environment<BADL_STIMULI_TYPES> const& /*unused*/,
      BADL::ComponentParams const& params, BADL::time time) noexcept -> bool;

  static void add_memory_actuator(BADL::Agent& agent,
                                  BADL::ProgramInterface const&,
                                  BADL::Environment<BADL_STIMULI_TYPES>&,
                                  BADL::ComponentParams const& params,
                                  BADL::time                   time);

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