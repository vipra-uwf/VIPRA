#include <gtest/gtest.h>

#include "vipra/types/line.hpp"

// TODO(rolland): add more tests

TEST(LineTests, DoIntersect) {
  // Test case 1: Lines intersect
  VIPRA::Line line1(VIPRA::f3d{0, 0}, VIPRA::f3d{2, 2});
  VIPRA::Line line2(VIPRA::f3d{1, 0}, VIPRA::f3d{1, 2});
  ASSERT_TRUE(line1.does_intersect(line2));

  // Test case 2: Lines do not intersect
  VIPRA::Line line3(VIPRA::f3d{0, 0}, VIPRA::f3d{2, 2});
  VIPRA::Line line4(VIPRA::f3d{3, 0}, VIPRA::f3d{3, 2});
  ASSERT_FALSE(line3.does_intersect(line4));

  // Test case 3: Lines are parallel
  VIPRA::Line line5(VIPRA::f3d{0, 0}, VIPRA::f3d{2, 2});
  VIPRA::Line line6(VIPRA::f3d{0, 1}, VIPRA::f3d{2, 3});
  ASSERT_FALSE(line5.does_intersect(line6));
}