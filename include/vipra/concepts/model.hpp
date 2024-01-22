#pragma once

#include <concepts>
#include <utility>

#include "vipra/concepts/obstacle_set.hpp"
#include "vipra/concepts/pedset.hpp"
#include "vipra/modules.hpp"

namespace VIPRA::Concepts {
template <typename model_t>
concept ModelModule = Module<model_t, VIPRA::Modules::Type::MODEL> &&
    requires(model_t model, const DummyPedSet& pedset, const DummyObsSet& obsset) {
  { model.timestep(pedset, obsset) } -> std::same_as<void>;
};
}  // namespace VIPRA::Concepts