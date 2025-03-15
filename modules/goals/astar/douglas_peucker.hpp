

#pragma once

#include <span>
#include <vector>

#include "vipra/geometry/f3d.hpp"

namespace VIPRA {

auto douglas_peucker_algo(std::span<VIPRA::f3d> path,
                          VIPRA::f_pnt          epsilon) -> std::vector<VIPRA::f3d>;

}  // namespace VIPRA