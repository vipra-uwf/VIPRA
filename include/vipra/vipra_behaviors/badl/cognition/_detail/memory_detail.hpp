#pragma once

#include <algorithm>
#include <type_traits>
#ifndef BADL_MEMORY_MAIN_HPP
#include "../memory.hpp"
#endif

namespace BADL {
template <typename... memory_ts>
template <typename memory_t>
void Memory<memory_ts...>::add_memory(memory_t&& memory,
                                      BADL::time timePercieved)
{
  auto& memoryContainer = get<memory_t>();
  memoryContainer.emplace_back(std::forward<memory_t>(memory), timePercieved);
}

template <typename... memory_ts>
template <typename memory_t>
auto Memory<memory_ts...>::has_memory() const noexcept -> bool
{
  return ! get<memory_t>().empty();
}

template <typename... memory_ts>
template <typename memory_t>
auto Memory<memory_ts...>::has_memory_since(BADL::time time) const noexcept
    -> bool
{
  auto const& container = get<memory_t>();
  return std::lower_bound(container.begin(), container.end(), time) !=
         container.end();
}

template <typename... memory_ts>
template <typename memory_t>
auto Memory<memory_ts...>::last_memory() const -> Instance<memory_t> const&
{
  assert(! get<memory_t>().empty());
  return get<memory_t>().back();
}

template <typename... memory_ts>
template <typename memory_t>
auto Memory<memory_ts...>::first_memory() const -> Instance<memory_t> const&
{
  assert(! get<memory_t>().empty());
  return get<memory_t>().front();
}

template <typename... memory_ts>
template <typename memory_t>
auto Memory<memory_ts...>::next_memory_after(BADL::time time) const
    -> std::optional<Instance<memory_t> const&>
{
  auto const& container = get<memory_t>();
  assert(! container.empty());

  auto iter = std::lower_bound(container.begin(), container.end(), time);
  if ( iter == container.end() ) return std::nullopt;

  return {*iter};
}

template <typename... memory_ts>
template <typename memory_t>
constexpr auto Memory<memory_ts...>::get() noexcept
    -> std::vector<Instance<std::remove_cvref_t<memory_t>>>&
{
  return std::get<std::vector<Instance<std::remove_cvref_t<memory_t>>>>(
      _memories);
}

template <typename... memory_ts>
template <typename memory_t>
constexpr auto Memory<memory_ts...>::get() const noexcept
    -> std::vector<Instance<std::remove_cvref_t<memory_t>>> const&
{
  return std::get<std::vector<Instance<std::remove_cvref_t<memory_t>>>>(
      _memories);
}
}  // namespace BADL
