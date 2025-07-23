#include "perth/doodson.hpp"

#include <gtest/gtest.h>

#include <iostream>

namespace perth {

TEST(DoodsonTest, CalculateCelestialVector) {
  // Test case with time = 45335 and delta = 53.026754231840584
  double time = 45335.0;
  double delta = 53.026754231840584;

  Vector6d result = calculate_celestial_vector(time, delta);

  // Expected values based on the provided test case
  Vector6d expected;
  expected << 160.59900704910373, 119.4907876655474, 280.08979471465113 - 360.0,
      111.5928443590156, 266.13901453365702 - 360.0, 282.64503464175243 - 360.0;

  // Use EXPECT_NEAR with appropriate tolerance for floating point comparison
  const double tolerance = 1e-10;

  EXPECT_NEAR(result[0], expected[0], tolerance);
  EXPECT_NEAR(result[1], expected[1], tolerance);
  EXPECT_NEAR(result[2], expected[2], tolerance);
  EXPECT_NEAR(result[3], expected[3], tolerance);
  EXPECT_NEAR(result[4], expected[4], tolerance);
  EXPECT_NEAR(result[5], expected[5], tolerance);
}

TEST(DoodsonTest, CalculateDoodsonArgument) {
  // Test case with time = 45335 and delta = 53.026754231840584
  constexpr double time = 45335.0;
  constexpr double delta = 53.026754231840584;

  // kNode constituent data
  const Data data = {{0, 0, 0, 0, 1, 0}, 2};

  double result = calculate_doodson_argument(time, delta, data);
  EXPECT_NEAR(result, 86.139014533657019, 1e-10);
}

}  // namespace perth
