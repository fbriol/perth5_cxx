#include "perth/love_numbers.hpp"

#include <gtest/gtest.h>

namespace perth {

TEST(LoveNumbersTest, LowFrequency) {
  auto [k2, h2, l2] = love_pmm95b(4.0);
  EXPECT_DOUBLE_EQ(k2, 0.299);
  EXPECT_DOUBLE_EQ(h2, 0.606);
  EXPECT_DOUBLE_EQ(l2, 0.0840);
}

TEST(LoveNumbersTest, HighFrequency) {
  auto [k2, h2, l2] = love_pmm95b(23.0);
  EXPECT_DOUBLE_EQ(k2, 0.302);
  EXPECT_DOUBLE_EQ(h2, 0.609);
  EXPECT_DOUBLE_EQ(l2, 0.0852);
}

TEST(LoveNumbersTest, MidFrequency) {
  auto [k2, h2, l2] = love_pmm95b(13.398660900971143);
  EXPECT_NEAR(k2, 0.29661217847085963, 1e-6);
  EXPECT_NEAR(h2, 0.60022176054190279, 1e-6);
  EXPECT_NEAR(l2, 0.083754256695820012, 1e-6);
}

}  // namespace perth
