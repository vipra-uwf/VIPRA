

#include <string>

#include "badl/agent.hpp"
#include "badl/components/memory/memory.hpp"

namespace BADL {
auto Memory::memory_map() -> std::map<std::string, size_t>&
{
  static std::map<std::string, size_t> memoryMap{};
  return memoryMap;
}

auto Memory::memory_id(std::string const& name) -> size_t
{
  return memory_map()[name];
}

void Memory::add_memory_actuator(
    BADL::Agent& agent, BADL::ProgramInterface const& /*unused*/,
    BADL::Environment<VIPRA::Sound, VIPRA::Sight>& /*unused*/,
    BADL::ComponentParams const& params, BADL::time time)
{
  agent.get_component<Memory>().add_memory(
      memory_id(params.get<std::string>(0)), time);
}

auto Memory::has_memory_condition(
    BADL::Agent const& agent, BADL::ProgramInterface const& /*unused*/,
    BADL::Environment<VIPRA::Sound, VIPRA::Sight> const& /*unused*/,
    BADL::ComponentParams const& params, BADL::time /*unused*/) noexcept -> bool
{
  size_t test = memory_id(params.get<std::string>(0));
  return agent.get_component<Memory>().has_memory(test);
}

void Memory::add_memory(size_t memoryId, BADL::time timePercieved)
{
  if ( ! _memories.contains(memoryId) ) _memories[memoryId] = {};
  _memories[memoryId].emplace_back(memoryId, timePercieved);
}

auto Memory::has_memory(size_t memoryId) const noexcept -> bool
{
  return _memories.contains(memoryId);
}

auto Memory::has_memory_since(size_t     memoryId,
                              BADL::time time) const noexcept -> bool
{
  auto const& container = get(memoryId);
  return std::lower_bound(container.begin(), container.end(), time) !=
         container.end();
}

auto Memory::last_memory(size_t memoryId) const -> MemoryInstance const&
{
  assert(! get(memoryId).empty());
  return get(memoryId).back();
}

auto Memory::first_memory(size_t memoryId) const -> MemoryInstance const&
{
  assert(! get(memoryId).empty());
  return get(memoryId).front();
}

auto Memory::next_memory_after(size_t memoryId, BADL::time time) const
    -> std::optional<MemoryInstance>
{
  auto const& container = get(memoryId);
  assert(! container.empty());

  auto iter = std::lower_bound(container.begin(), container.end(), time);
  if ( iter == container.end() ) return std::nullopt;

  return {*iter};
}

auto Memory::get(size_t memoryId) noexcept -> std::vector<MemoryInstance>&
{
  if ( ! _memories.contains(memoryId) ) _memories[memoryId] = {};
  return _memories[memoryId];
}

auto Memory::get(size_t memoryId) const noexcept
    -> std::vector<MemoryInstance> const&
{
  return get(memoryId);
}
}  // namespace BADL