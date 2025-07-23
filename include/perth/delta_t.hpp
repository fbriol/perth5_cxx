#pragma once

#include <cmath>
#include <stdexcept>
#include <string>

#include "perth/math.hpp"

namespace perth {

/// @brief Computes the difference between Universal Time (UT) and Terrestrial
/// Dynamical Time (TT), also known as the old Ephemeris Time (ET).
/// @param[in] tj Julian date in days (e.g., 2010 Jan 1 noon = 2455198).
/// @return The difference ET - UT in seconds.
/// @note The output is an approximation based on polynomial tables from work by
/// Espenak and Meeus. It is starting to get slightly off as it was based
/// on observed data only through ~2010.
PERTH_MATH_CONSTEXPR auto calculate_delta_time(double tj) -> double {
  auto y = std::round((tj - 2415020.0) / 365.25) + 1900;  // Year

  // Ensure the year is within the valid range for the Delta_T routine.
  if (y < 1700 || y > 2150) {
    throw std::out_of_range(
        "Time out of range in Delta_T routine; revise for year " +
        std::to_string(y));
  }

  if (y >= 2050) {
    return -20.0 + 32.0 * pow<2, double>((y - 1820) / 100.0) -
           0.5628 * (2150 - y);
  }

  if (y >= 2005) {
    auto t = y - 2000;
    return 62.92 + 0.32217 * t + 5.5589e-3 * pow<2, double>(t);
  }

  if (y >= 1986) {
    auto t = y - 2000;
    return ((((2.373599e-5 * t + 6.51814e-4) * t + 1.7275e-3) * t - 6.0374e-2) *
                t +
            0.3345) *
               t +
           63.86;
  }

  if (y >= 1961) {
    auto t = y - 1975;
    return 45.45 + 1.067 * t - t * t / 260.0 - pow<3, double>(t) / 718.0;
  }

  if (y >= 1941) {
    auto t = y - 1950;
    return 29.07 + 0.407 * t - t * t / 233.0 + pow<3, double>(t) / 2547.0;
  }

  if (y >= 1920) {
    auto t = y - 1920;
    return 21.20 + 0.84493 * t - 0.0761 * pow<2, double>(t) +
           2.0936e-3 * pow<3, double>(t);
  }

  if (y >= 1900) {
    auto t = y - 1900;
    return -2.79 + 1.494119 * t - 5.98939e-2 * pow<2, double>(t) +
           6.1966e-3 * pow<3, double>(t) - 1.97e-4 * pow<4, double>(t);
  }

  if (y >= 1860) {
    auto t = y - 1860;
    return ((((4.28864e-6 * t - 4.473624e-4) * t + 1.680668e-2) * t -
             2.51754e-1) *
                t +
            0.5737) *
               t +
           7.62;
  }

  if (y >= 1800) {
    auto t = y - 1800;
    return ((((((8.75e-10 * t - 1.699e-7) * t + 1.21272e-5) * t - 3.7436e-4) *
                  t +
              4.1116e-3) *
                 t +
             6.861e-3) *
                t -
            0.332447) *
               t +
           13.72;
  }

  // (y >= 1700) ?
  auto t = y - 1700;
  return 8.83 + 1.603e-1 * t - 5.9285e-3 * pow<2, double>(t) +
         1.3336e-4 * pow<3, double>(t) - 8.518e-7 * pow<4, double>(t);
}

}  // namespace perth
