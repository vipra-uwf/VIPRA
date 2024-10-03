#pragma once

#include "vipra/geometry/rectangle.hpp"
namespace VIPRA::Behaviors {

/**
   * @brief A Location is an area in the map that is either predefined, such as the bathroom, or defined by a behavior created by the user.
   *     
   */
using Location = Geometry::Rectangle;
}  //namespace VIPRA::Behaviors