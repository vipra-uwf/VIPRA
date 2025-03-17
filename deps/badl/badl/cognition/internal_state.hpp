#pragma once

#include "badl/components/base_component.hpp"
#include "badl/components/component.hpp"

namespace BADL {
template <typename... component_ts>
class InternalState
    : public BADL::Component,
      public BADL::BaseComponent<InternalState<component_ts...>> {
 public:
 private:
  std::tuple<component_ts...> _components;
};
}  // namespace BADL