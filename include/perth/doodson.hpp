#pragma once

#include <Eigen/Dense>

#include "perth/constituent.hpp"
#include "perth/eigen.hpp"
#include "perth/fundarg.hpp"
#include "perth/math.hpp"

namespace perth {

/// @brief Evaluate Doodson's 6 astronomical variables.
/// Computed angles are returned in units of degrees, between [0,360).
/// \f$\text{vector} = [ \tau, s, h, p, N^{\prime}, p_s ]\f$
/// @param[in] time Universal Time in decimal Modified Julian Days.
/// @param[in] delta Delta T, in seconds.
auto calculate_celestial_vector(double time, double delta) noexcept -> Vector6d;

/// @brief Evaluate Doodson's tidal argument at a given time.
/// @param[in] time Universal Time in decimal Modified Julian Days.
/// @param[in] delta Delta T, in seconds.
/// @param[in] component Tide component to evaluate.
/// @return Doodson's tidal argument
inline auto calculate_doodson_argument(
    double time, double delta, const Eigen::Ref<const Vector7d>& doodson_number)
    -> double {
  Eigen::Vector<double, 7> beta;
  beta << calculate_celestial_vector(time, delta), 90.0;

  // Compute argument as dot product with Doodson number
  double arg = doodson_number.dot(beta);
  return normalize_angle(arg);
}

}  // namespace perth
