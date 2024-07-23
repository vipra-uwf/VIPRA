

#include <gtest/gtest.h>

#include "vipra/parameter_sweep/parameter_sweep.hpp"
#include "vipra/parameter_sweep/ps_util.hpp"

TEST(ParameterSweepTests, SimCounts) {
  // Evenly Divisible
  EXPECT_EQ(VIPRA::sim_count(0, 4, 16), 4);
  EXPECT_EQ(VIPRA::sim_count(1, 4, 16), 4);
  EXPECT_EQ(VIPRA::sim_count(2, 4, 16), 4);
  EXPECT_EQ(VIPRA::sim_count(3, 4, 16), 4);

  EXPECT_EQ(VIPRA::start_sim_id(0, 4, 16), 0);
  EXPECT_EQ(VIPRA::start_sim_id(1, 4, 16), 4);
  EXPECT_EQ(VIPRA::start_sim_id(2, 4, 16), 8);
  EXPECT_EQ(VIPRA::start_sim_id(3, 4, 16), 12);

  EXPECT_TRUE(VIPRA::start_sim_id(0, 4, 16) + VIPRA::sim_count(0, 4, 16) == VIPRA::start_sim_id(1, 4, 16));
  EXPECT_TRUE(VIPRA::start_sim_id(1, 4, 16) + VIPRA::sim_count(1, 4, 16) == VIPRA::start_sim_id(2, 4, 16));
  EXPECT_TRUE(VIPRA::start_sim_id(2, 4, 16) + VIPRA::sim_count(2, 4, 16) == VIPRA::start_sim_id(3, 4, 16));

  // Evenly Divisible
  EXPECT_EQ(VIPRA::sim_count(0, 4, 20), 5);
  EXPECT_EQ(VIPRA::sim_count(1, 4, 20), 5);
  EXPECT_EQ(VIPRA::sim_count(2, 4, 20), 5);
  EXPECT_EQ(VIPRA::sim_count(3, 4, 20), 5);

  EXPECT_EQ(VIPRA::start_sim_id(0, 4, 20), 0);
  EXPECT_EQ(VIPRA::start_sim_id(1, 4, 20), 5);
  EXPECT_EQ(VIPRA::start_sim_id(2, 4, 20), 10);
  EXPECT_EQ(VIPRA::start_sim_id(3, 4, 20), 15);

  EXPECT_TRUE(VIPRA::start_sim_id(0, 4, 20) + VIPRA::sim_count(0, 4, 20) == VIPRA::start_sim_id(1, 4, 20));
  EXPECT_TRUE(VIPRA::start_sim_id(1, 4, 20) + VIPRA::sim_count(1, 4, 20) == VIPRA::start_sim_id(2, 4, 20));
  EXPECT_TRUE(VIPRA::start_sim_id(2, 4, 20) + VIPRA::sim_count(2, 4, 20) == VIPRA::start_sim_id(3, 4, 20));

  // Not evenly divisible
  EXPECT_EQ(VIPRA::sim_count(0, 4, 17), 5);
  EXPECT_EQ(VIPRA::sim_count(1, 4, 17), 4);
  EXPECT_EQ(VIPRA::sim_count(2, 4, 17), 4);
  EXPECT_EQ(VIPRA::sim_count(3, 4, 17), 4);

  EXPECT_EQ(VIPRA::start_sim_id(0, 4, 17), 0);
  EXPECT_EQ(VIPRA::start_sim_id(1, 4, 17), 5);
  EXPECT_EQ(VIPRA::start_sim_id(2, 4, 17), 9);
  EXPECT_EQ(VIPRA::start_sim_id(3, 4, 17), 13);

  EXPECT_TRUE(VIPRA::start_sim_id(0, 4, 17) + VIPRA::sim_count(0, 4, 17) == VIPRA::start_sim_id(1, 4, 17));
  EXPECT_TRUE(VIPRA::start_sim_id(1, 4, 17) + VIPRA::sim_count(1, 4, 17) == VIPRA::start_sim_id(2, 4, 17));
  EXPECT_TRUE(VIPRA::start_sim_id(2, 4, 17) + VIPRA::sim_count(2, 4, 17) == VIPRA::start_sim_id(3, 4, 17));

  // Not evenly divisible
  EXPECT_EQ(VIPRA::sim_count(0, 4, 18), 5);
  EXPECT_EQ(VIPRA::sim_count(1, 4, 18), 5);
  EXPECT_EQ(VIPRA::sim_count(2, 4, 18), 4);
  EXPECT_EQ(VIPRA::sim_count(3, 4, 18), 4);

  EXPECT_EQ(VIPRA::start_sim_id(0, 4, 18), 0);
  EXPECT_EQ(VIPRA::start_sim_id(1, 4, 18), 5);
  EXPECT_EQ(VIPRA::start_sim_id(2, 4, 18), 10);
  EXPECT_EQ(VIPRA::start_sim_id(3, 4, 18), 14);

  EXPECT_TRUE(VIPRA::start_sim_id(0, 4, 18) + VIPRA::sim_count(0, 4, 18) == VIPRA::start_sim_id(1, 4, 18));
  EXPECT_TRUE(VIPRA::start_sim_id(1, 4, 18) + VIPRA::sim_count(1, 4, 18) == VIPRA::start_sim_id(2, 4, 18));
  EXPECT_TRUE(VIPRA::start_sim_id(2, 4, 18) + VIPRA::sim_count(2, 4, 18) == VIPRA::start_sim_id(3, 4, 18));

  // Not evenly divisible
  EXPECT_EQ(VIPRA::sim_count(0, 4, 19), 5);
  EXPECT_EQ(VIPRA::sim_count(1, 4, 19), 5);
  EXPECT_EQ(VIPRA::sim_count(2, 4, 19), 5);
  EXPECT_EQ(VIPRA::sim_count(3, 4, 19), 4);

  EXPECT_EQ(VIPRA::start_sim_id(0, 4, 19), 0);
  EXPECT_EQ(VIPRA::start_sim_id(1, 4, 19), 5);
  EXPECT_EQ(VIPRA::start_sim_id(2, 4, 19), 10);
  EXPECT_EQ(VIPRA::start_sim_id(3, 4, 19), 15);

  EXPECT_TRUE(VIPRA::start_sim_id(0, 4, 19) + VIPRA::sim_count(0, 4, 19) == VIPRA::start_sim_id(1, 4, 19));
  EXPECT_TRUE(VIPRA::start_sim_id(1, 4, 19) + VIPRA::sim_count(1, 4, 19) == VIPRA::start_sim_id(2, 4, 19));
  EXPECT_TRUE(VIPRA::start_sim_id(2, 4, 19) + VIPRA::sim_count(2, 4, 19) == VIPRA::start_sim_id(3, 4, 19));
}
