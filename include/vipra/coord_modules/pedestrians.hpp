#pragma once

#include <tuple>

#include "vipra/concepts/pedset.hpp"

namespace VIPRA {

template <Concepts::PedsetModule... pedset_ts>
class Pedestrians {
  VIPRA_MODULE_TYPE(PEDESTRIANS);

 public:
 private:
  std::tuple<pedset_ts...> _pedestrians;
};

CHECK_MODULE(PedsetModule, Pedestrians<Concepts::DummyPedSet>);

}  // namespace VIPRA