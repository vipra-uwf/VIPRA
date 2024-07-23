#pragma once

#include "vipra/concepts/is_void.hpp"

namespace VIPRA::Concepts {
template <typename class_t>
concept has_parameters = requires(class_t instance)
{
  {instance.parameters()};
};
}  // namespace VIPRA::Concepts