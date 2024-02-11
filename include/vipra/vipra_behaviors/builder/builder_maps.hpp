#pragma once

#include <string>
#include <unordered_map>

#include "vipra/types/idx.hpp"

#include "vipra/vipra_behaviors/definitions/dsl_types.hpp"
#include "vipra/vipra_behaviors/definitions/pedestrian_types.hpp"
#include "vipra/vipra_behaviors/util/caseless_str_comp.hpp"

namespace VIPRA::Behaviors {
using StateMap =
    std::unordered_map<std::string, Behaviors::stateUID, CaselessStrCompare::Hash, CaselessStrCompare::Comp>;
using TypeMap =
    std::unordered_map<std::string, Behaviors::typeUID, CaselessStrCompare::Hash, CaselessStrCompare::Comp>;
using EventMap =
    std::unordered_map<std::string, VIPRA::idx, CaselessStrCompare::Hash, CaselessStrCompare::Comp>;
using LocationMap =
    std::unordered_map<std::string, VIPRA::idx, CaselessStrCompare::Hash, CaselessStrCompare::Comp>;
}  // namespace VIPRA::Behaviors
