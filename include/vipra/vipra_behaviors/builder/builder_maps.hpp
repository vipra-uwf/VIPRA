#ifndef VIPRA_BEHAVIORS_BUILDER_MAPS_HPP
#define VIPRA_BEHAVIORS_BUILDER_MAPS_HPP

#include <string>
#include <unordered_map>

#include <definitions/dsl_types.hpp>
#include <definitions/pedestrian_types.hpp>
#include <util/caseless_str_comp.hpp>

namespace BHVR {
using StateMap =
    std::unordered_map<std::string, BHVR::stateUID, CaselessStrCompare::Hash, CaselessStrCompare::Comp>;
using TypeMap =
    std::unordered_map<std::string, BHVR::typeUID, CaselessStrCompare::Hash, CaselessStrCompare::Comp>;
using EventMap =
    std::unordered_map<std::string, VIPRA::idx, CaselessStrCompare::Hash, CaselessStrCompare::Comp>;
using LocationMap =
    std::unordered_map<std::string, VIPRA::idx, CaselessStrCompare::Hash, CaselessStrCompare::Comp>;
}  // namespace BHVR

#endif