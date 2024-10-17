
#include <gtest/gtest.h>
#include <vector>

#include "vipra/geometry/polygon.hpp"

// NOLINTBEGIN

TEST(PolygonTest, Constructors)
{
  using namespace VIPRA;
  using namespace VIPRA::Geometry;

  std::array<f3d, 6> hexPointsArr{VIPRA::f3d{}, VIPRA::f3d{}, VIPRA::f3d{},
                                  VIPRA::f3d{}, VIPRA::f3d{}, VIPRA::f3d{}};
  std::vector<f3d>   hexPointsVec(6);

  Polygon arrPoly1(hexPointsArr);
  Polygon arrPoly2(hexPointsVec);

  Polygon vecPoly1(hexPointsArr);
  Polygon vecPoly2(hexPointsVec);

  Polygon vecPoly3(arrPoly1);
  Polygon arrPoly3(vecPoly1);
}

// NOLINTEND