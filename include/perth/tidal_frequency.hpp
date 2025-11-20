#pragma once

#include "perth/doodson.hpp"
#include "perth/eigen.hpp"

namespace perth {

/// @brief Compute the frequency of a tide with a given Doodson number
/// (without the 5's). The returned frequency is in units of degrees per hour.
/// @param[in] doodson_number Doodson number as a 6-dimensional vector.
/// @return Frequency in degrees per hour.
inline auto tidal_frequency(const Eigen::Ref<const Vector6d> &doodson_number)
    -> double {
  // Time interval in days
  constexpr double del = 0.05;

  // Julian Day at J2000
  constexpr double t1 = 51545.0;
  constexpr double t2 = t1 + del;

  // Compute Doodson vectors at two times
  const auto beta1 = calculate_celestial_vector(t1, 0.0);
  const auto beta2 = calculate_celestial_vector(t2, 0.0);

  // Compute rates in degrees per hour
  const auto rates = (beta2 - beta1) / (24.0 * del);

  // Compute frequency as dot product with Doodson number
  return rates.dot(doodson_number);
}

}  // namespace perth
