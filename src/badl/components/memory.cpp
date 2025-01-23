

#include "badl/cognition/memory.hpp"
#include "badl/agent.hpp"

namespace BADL {
auto Memory::has_memory_condition(
    BADL::Agent const& agent, BADL::ProgramInterface const& /*unused*/,
    BADL::Environment<VIPRA::Sound, VIPRA::Sight> const& /*unused*/,
    BADL::ComponentParams const& params) noexcept -> bool
{
  auto test = params.get<float>(0);
  return agent.get_memory(test);
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