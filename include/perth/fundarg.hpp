#pragma once

#include "perth/math.hpp"

namespace perth {

/// @brief Fundamental arguments for the lunisolar model.
struct Arguments {
  double l;   ///< Mean anomaly of the Moon (cf. note 2)
  double lp;  ///< Mean anomaly of the Sun (cf. note 2)
  double f;   ///< L - OM (Mean longitude of the Moon) (cf. notes 2 and 3)
  double d;   ///< Mean elongation of the Moon from the Sun (cf. note 2)
  double om;  ///< Mean longitude of the ascending node of the Moon (cf. note 2)
};

/// @brief Computes the lunisolar fundamental arguments. The model used is from
/// Simon et al. (1994) as recommended by the IERS Conventions (2010) Chapter 5
/// Sections 5.7.1 - 5.7.2 (pp. 57 - 59).
///
/// In general, Class 1, 2, and 3 models represent physical effects that
/// act on geodetic parameters while canonical models provide lower-level
/// representations or basic computations that are used by Class 1, 2, or
/// 3 models.
///
/// Status: Canonical model
/// * Class 1 models are those recommended to be used a priori in the
///   reduction of raw space geodetic data in order to determine
///   geodetic parameter estimates.
/// * Class 2 models are those that eliminate an observational
///   singularity and are purely conventional in nature.
/// * Class 3 models are those that are not required as either Class
///   1 or 2.
/// * Canonical models are accepted as is and cannot be classified as a
///   Class 1, 2, or 3 model.
/// @param[in] time Universal Time in Julian centuries since J2000. (cf. note 1)
/// @return The fundamental arguments in radians.
///
/// @note 1. Though time is strictly TDB, it is usually more convenient to use
/// TT, which makes no significant difference. Julian centuries since
/// J2000 is (JD - 2451545.0)/36525.
/// @note 2. The expression used is as adopted in IERS Conventions (2010) and
/// is from Simon et al. (1994). Arguments are in radians.
/// @note 3. l in this instance is the Mean Longitude of the Moon. om is the
/// Mean longitude of the ascending node of the Moon.
/// @cite Simon, J.-L., Bretagnon, P., Chapront, J., Chapront-Touze, M.,
/// Francou, G., Laskar, J., 1994, Astron.Astrophys. 282, 663-683
/// @cite Petit, G. and Luzum, B. (eds.), IERS Conventions (2010),
/// IERS Technical Note No. 36, BKG (2010)
PERTH_MATH_CONSTEXPR auto fundarg(double time) noexcept -> Arguments {
  // Arcseconds in a full circle
  constexpr auto arcseconds_in_circle = 1296000.0;

  // Mean anomaly of the moon (L)
  auto l = arcseconds2radians(
      std::remainder(horner(time, 485868.249036, 1717915923.2178, 31.8792,
                            0.051635, -0.00024470),
                     arcseconds_in_circle));

  // Mean anomaly of the sun (LP)
  auto lp = arcseconds2radians(
      std::remainder(horner(time, 1287104.79305, 129596581.0481, -0.5532,
                            0.000136, -0.00001149),
                     arcseconds_in_circle));

  // L - OM (F)
  auto f = arcseconds2radians(
      std::remainder(horner(time, 335779.526232, 1739527262.8478, -12.7512,
                            -0.001037, 0.00000417),
                     arcseconds_in_circle));

  // Mean elongation of the moon from the sun (D)
  auto d = arcseconds2radians(
      std::remainder(horner(time, 1072260.70369, 1602961601.2090, -6.3706,
                            0.006593, -0.00003169),
                     arcseconds_in_circle));

  // Mean longitude of the ascending node of the moon (OM)
  auto om = arcseconds2radians(std::remainder(
      horner(time, 450160.398036, -6962890.5431, 7.4722, 0.007702, -0.00005939),
      arcseconds_in_circle));

  return Arguments{l, lp, f, d, om};
}

}  // namespace perth
