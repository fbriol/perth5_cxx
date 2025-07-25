#include "perth/delta_t.hpp"

#include <gtest/gtest.h>

#include <cmath>
#include <stdexcept>

#include "perth/datetime.hpp"

namespace perth {

// Test fixture for delta_t tests
class DeltaTTest : public ::testing::Test {
 protected:
  constexpr double year_to_modified_julian_date(int year) {
    int month = 1;  // January
    int day = 1;

    // Adjust for January/February in Julian calendar algorithm
    int a = (14 - month) / 12;
    int y = year - a;
    int m = month + 12 * a - 3;

    // Julian Day Number calculation
    int jdn =
        day + (153 * m + 2) / 5 + 365 * y + y / 4 - y / 100 + y / 400 + 1721119;

    // Convert to Modified Julian Date (JD - 2400000.5, and subtract 0.5 for
    // midnight)
    return jdn - 2400001.0;
  }

  // Helper function to convert year to Julian date
  constexpr double year_to_julian_date(int year) {
    return year_to_modified_julian_date(year) + kModifiedJulianEpoch;
  }

  // Tolerance for floating point comparisons
  static constexpr double kTolerance = 1e-6;
};

// Test valid year ranges and boundary conditions
TEST_F(DeltaTTest, ValidYearRanges) {
  // Test boundary years that should work
  EXPECT_NO_THROW(calculate_delta_time(year_to_julian_date(1700)));
  EXPECT_NO_THROW(calculate_delta_time(year_to_julian_date(1701)));
  EXPECT_NO_THROW(calculate_delta_time(year_to_julian_date(2149)));
  EXPECT_NO_THROW(calculate_delta_time(year_to_julian_date(2150)));

  // Test some years in between
  EXPECT_NO_THROW(calculate_delta_time(year_to_julian_date(1800)));
  EXPECT_NO_THROW(calculate_delta_time(year_to_julian_date(1900)));
  EXPECT_NO_THROW(calculate_delta_time(year_to_julian_date(2000)));
  EXPECT_NO_THROW(calculate_delta_time(year_to_julian_date(2020)));
}

// Test invalid year ranges
TEST_F(DeltaTTest, InvalidYearRanges) {
  // Test years before 1700
  EXPECT_THROW(calculate_delta_time(year_to_julian_date(1699)),
               std::out_of_range);
  EXPECT_THROW(calculate_delta_time(year_to_julian_date(1500)),
               std::out_of_range);

  // Test years after 2150
  EXPECT_THROW(calculate_delta_time(year_to_julian_date(2151)),
               std::out_of_range);
  EXPECT_THROW(calculate_delta_time(year_to_julian_date(2200)),
               std::out_of_range);
}

// Test specific known values for historical validation
TEST_F(DeltaTTest, HistoricalKnownValues) {
  // Test some approximate known values from different time periods
  // These are rough approximations based on the polynomial formulas

  // Year 1700 (oldest range)
  double dt_1700 = calculate_delta_time(year_to_julian_date(1700));
  EXPECT_NEAR(dt_1700, 8.83, 1e-6);

  // Year 1800
  double dt_1800 = calculate_delta_time(year_to_julian_date(1800));
  EXPECT_NEAR(dt_1800, 13.72, 1e-6);

  // Year 1900
  double dt_1900 = calculate_delta_time(year_to_julian_date(1900));
  EXPECT_NEAR(dt_1900, -2.79, 1e-6);

  // Year 2000 (modern era)
  double dt_2000 = calculate_delta_time(year_to_julian_date(2000));
  EXPECT_NEAR(dt_2000, 63.86, 1e-6);
}

// Test polynomial ranges to ensure proper branch coverage
TEST_F(DeltaTTest, PolynomialRangeCoverage) {
  // Test each polynomial range defined in the function

  // 1700-1799 range
  double dt_1750 = calculate_delta_time(year_to_julian_date(1750));
  EXPECT_NEAR(dt_1750, 13.37, 1e-6);

  // 1800-1859 range
  double dt_1850 = calculate_delta_time(year_to_julian_date(1850));
  EXPECT_NEAR(dt_1850, 7.1064, 1e-6);

  // 1860-1899 range
  double dt_1880 = calculate_delta_time(year_to_julian_date(1880));
  // Delta T can be negative in this historical period
  EXPECT_NEAR(dt_1880, -5.008496, 1e-6);

  // 1900-1919 range
  double dt_1910 = calculate_delta_time(year_to_julian_date(1910));
  EXPECT_NEAR(dt_1910, 10.3884, 1e-6);

  // 1920-1940 range
  double dt_1930 = calculate_delta_time(year_to_julian_date(1930));
  EXPECT_NEAR(dt_1930, 24.1329, 1e-6);

  // 1941-1960 range
  double dt_1950 = calculate_delta_time(year_to_julian_date(1950));
  EXPECT_NEAR(dt_1950, 29.07, 1e-6);

  // 1961-1985 range
  double dt_1975 = calculate_delta_time(year_to_julian_date(1975));
  EXPECT_NEAR(dt_1975, 45.45, 1e-6);

  // 1986-2004 range
  double dt_1995 = calculate_delta_time(year_to_julian_date(1995));
  EXPECT_NEAR(dt_1995, 60.79542128125, 1e-6);

  // 2005-2049 range
  double dt_2010 = calculate_delta_time(year_to_julian_date(2010));
  EXPECT_NEAR(dt_2010, 66.69759, 1e-6);

  // 2050+ range
  double dt_2100 = calculate_delta_time(year_to_julian_date(2100));
  EXPECT_NEAR(dt_2100, 202.74, 1e-6);
}

// Test boundary transitions between polynomial ranges
TEST_F(DeltaTTest, PolynomialBoundaryTransitions) {
  // Test values right at the boundary between ranges for continuity

  // Around 1800 boundary
  double dt_1799 = calculate_delta_time(year_to_julian_date(1799));
  double dt_1800 = calculate_delta_time(year_to_julian_date(1800));
  // Values should be reasonably close (within a few seconds)
  EXPECT_LT(std::abs(dt_1800 - dt_1799), 5.0);

  // Around 1900 boundary
  double dt_1899 = calculate_delta_time(year_to_julian_date(1899));
  double dt_1900 = calculate_delta_time(year_to_julian_date(1900));
  EXPECT_LT(std::abs(dt_1900 - dt_1899), 5.0);

  // Around 2000 boundary
  double dt_1999 = calculate_delta_time(year_to_julian_date(1999));
  double dt_2000 = calculate_delta_time(year_to_julian_date(2000));
  EXPECT_LT(std::abs(dt_2000 - dt_1999), 5.0);
}

// Test monotonicity in recent years (Delta T generally increases)
TEST_F(DeltaTTest, ModernEraMonotonicity) {
  // In the modern era, Delta T should generally increase
  double dt_1990 = calculate_delta_time(year_to_julian_date(1990));
  double dt_2000 = calculate_delta_time(year_to_julian_date(2000));
  double dt_2010 = calculate_delta_time(year_to_julian_date(2010));
  double dt_2020 = calculate_delta_time(year_to_julian_date(2020));

  EXPECT_LT(dt_1990, dt_2000);
  EXPECT_LT(dt_2000, dt_2010);
  EXPECT_LT(dt_2010, dt_2020);
}

// Test with specific Julian dates (not just year boundaries)
TEST_F(DeltaTTest, SpecificJulianDates) {
  // Test with the example given in the comment: 2010 Jan 1 noon = 2455198
  double dt_2010_jan1 = calculate_delta_time(2455198.0);
  EXPECT_GT(dt_2010_jan1, 65.0);
  EXPECT_LT(dt_2010_jan1, 70.0);

  // Test with some other specific dates
  double dt_mid_year = calculate_delta_time(2455198.0 + 182.5);  // Mid 2010
  EXPECT_GT(dt_mid_year, 65.0);
  EXPECT_LT(dt_mid_year, 70.0);

  // Should be very close to the Jan 1 value since the polynomial changes slowly
  EXPECT_LT(std::abs(dt_mid_year - dt_2010_jan1), 1.0);
}

// Test numerical stability with fractional Julian dates
TEST_F(DeltaTTest, FractionalJulianDates) {
  double base_date = year_to_julian_date(2000);

  // Test with various fractional parts
  double dt_base = calculate_delta_time(base_date);
  double dt_quarter = calculate_delta_time(base_date + 0.25);
  double dt_half = calculate_delta_time(base_date + 0.5);
  double dt_three_quarter = calculate_delta_time(base_date + 0.75);

  // All should be very close since the function depends on the year, not the
  // exact date
  EXPECT_NEAR(dt_base, dt_quarter, 0.01);
  EXPECT_NEAR(dt_base, dt_half, 0.01);
  EXPECT_NEAR(dt_base, dt_three_quarter, 0.01);
}

// Test extreme values within valid range
TEST_F(DeltaTTest, ExtremeValidValues) {
  // Test the extreme ends of the valid range
  double dt_1700 = calculate_delta_time(year_to_julian_date(1700));
  double dt_2150 = calculate_delta_time(year_to_julian_date(2150));

  // Both should be finite and reasonable
  EXPECT_TRUE(std::isfinite(dt_1700));
  EXPECT_TRUE(std::isfinite(dt_2150));
  EXPECT_GT(dt_1700, -100.0);  // Should not be extremely negative
  EXPECT_LT(dt_1700, 100.0);   // Should not be extremely positive
  EXPECT_GT(dt_2150, -100.0);
  EXPECT_LT(dt_2150, 1000.0);  // Future extrapolation might be larger
}

// Test return value characteristics
TEST_F(DeltaTTest, ReturnValueCharacteristics) {
  // Test several dates to ensure return values are reasonable
  std::vector<int> test_years = {1750, 1850, 1900, 1950, 2000, 2050, 2100};

  for (int year : test_years) {
    double dt = calculate_delta_time(year_to_julian_date(year));

    // Should always be finite
    EXPECT_TRUE(std::isfinite(dt));
    EXPECT_FALSE(std::isnan(dt));

    // Should be within reasonable bounds (historically, Delta T ranges roughly
    // from -20 to +100 seconds) But extrapolation to far future can produce
    // larger values
    EXPECT_GT(dt, -30.0);
    EXPECT_LT(dt,
              300.0);  // Allow for larger extrapolation values in far future
  }
}

// Test precision and consistency
TEST_F(DeltaTTest, PrecisionConsistency) {
  double test_date = year_to_julian_date(2000);

  // Call the function multiple times with the same input
  double dt1 = calculate_delta_time(test_date);
  double dt2 = calculate_delta_time(test_date);
  double dt3 = calculate_delta_time(test_date);

  // Results should be identical (deterministic function)
  EXPECT_DOUBLE_EQ(dt1, dt2);
  EXPECT_DOUBLE_EQ(dt2, dt3);

  // Small changes in input should produce small changes in output
  double dt_plus = calculate_delta_time(test_date + 1.0);  // Next day
  EXPECT_LT(std::abs(dt_plus - dt1),
            0.1);  // Should change very little day to day
}

// Test error message content for out of range years
TEST_F(DeltaTTest, ErrorMessageContent) {
  try {
    calculate_delta_time(year_to_julian_date(1650));
    FAIL() << "Expected std::out_of_range exception";
  } catch (const std::out_of_range& e) {
    std::string error_msg(e.what());
    EXPECT_TRUE(error_msg.find("Time out of range") != std::string::npos);
    EXPECT_TRUE(error_msg.find("1650") != std::string::npos);
  }

  try {
    calculate_delta_time(year_to_julian_date(2200));
    FAIL() << "Expected std::out_of_range exception";
  } catch (const std::out_of_range& e) {
    std::string error_msg(e.what());
    EXPECT_TRUE(error_msg.find("Time out of range") != std::string::npos);
    EXPECT_TRUE(error_msg.find("2200") != std::string::npos);
  }
}

// Test specific polynomial coefficients by testing characteristic values
TEST_F(DeltaTTest, PolynomialCharacteristics) {
  // Test that each polynomial range produces expected characteristics

  // 1700-1799: Should show increasing trend
  double dt_1710 = calculate_delta_time(year_to_julian_date(1710));
  double dt_1750 = calculate_delta_time(year_to_julian_date(1750));
  double dt_1790 = calculate_delta_time(year_to_julian_date(1790));
  EXPECT_LT(dt_1710, dt_1750);
  EXPECT_LT(dt_1750, dt_1790);

  // 1900-1919: Should show the characteristic minimum around early 1900s
  double dt_1900 = calculate_delta_time(year_to_julian_date(1900));
  double dt_1905 = calculate_delta_time(year_to_julian_date(1905));
  double dt_1910 = calculate_delta_time(year_to_julian_date(1910));
  // This period has negative Delta T values
  EXPECT_LT(dt_1900, 5.0);  // Should be small or negative

  // 2050+: Should show rapid increase (extrapolation formula)
  double dt_2050 = calculate_delta_time(year_to_julian_date(2050));
  double dt_2100 = calculate_delta_time(year_to_julian_date(2100));
  EXPECT_LT(dt_2050, dt_2100);         // Should increase significantly
  EXPECT_GT(dt_2100 - dt_2050, 50.0);  // Should be a substantial increase
}

// Test mathematical properties of the function
TEST_F(DeltaTTest, MathematicalProperties) {
  // Test that the function is well-behaved mathematically
  std::vector<double> test_dates;
  for (int year = 1700; year <= 2150; year += 10) {
    test_dates.push_back(year_to_julian_date(year));
  }

  for (double date : test_dates) {
    double dt = calculate_delta_time(date);

    // Should never produce NaN or infinity
    EXPECT_TRUE(std::isfinite(dt));
    EXPECT_FALSE(std::isnan(dt));

    // Should be within physically reasonable bounds
    // (Earth's rotation can't differ by more than a few minutes from atomic
    // time)
    EXPECT_GT(dt, -100.0);  // About -1.7 minutes
    EXPECT_LT(
        dt,
        500.0);  // About 8.3 minutes (allowing for far future extrapolation)
  }
}

// Test year conversion accuracy
TEST_F(DeltaTTest, YearConversionAccuracy) {
  // Test that our year_to_julian_date helper produces reasonable results
  // by testing against known Julian date conversions

  // Test the example from the function comment: 2010 Jan 1 noon = 2455198
  double computed_2010 = year_to_julian_date(2010);
  EXPECT_NEAR(computed_2010, 2455198.0, 1.0);  // Should be within 1 day

  // Test another known conversion: J2000.0 (Jan 1.5, 2000) = 2451545.0
  double computed_2000 = year_to_julian_date(2000);
  EXPECT_NEAR(computed_2000, 2451545.0, 1.0);  // Should be within 1 day
}

// Test computational stability with repeated calls
TEST_F(DeltaTTest, ComputationalStability) {
  double test_date = year_to_julian_date(2000);

  // Call function many times and ensure results remain stable
  std::vector<double> results;
  for (int i = 0; i < 1000; ++i) {
    results.push_back(calculate_delta_time(test_date));
  }

  // All results should be identical
  for (size_t i = 1; i < results.size(); ++i) {
    EXPECT_DOUBLE_EQ(results[0], results[i]);
  }
}

}  // namespace perth
