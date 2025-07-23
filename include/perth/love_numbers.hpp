#pragma once

#include <tuple>

namespace perth {

/// @brief Computes the Love numbers for a given tidal frequency using the
/// PMM95B model (Mathews et al., 1995).
/// @param frequency Tidal frequency in degrees per hour.
/// @return A tuple containing the Love numbers k2, h2, and l2.
/// @note This function uses the abbreviated model from Mathews et al. (1995)
/// for diurnal Love numbers. It returns Wahr's 1981 numbers for frequencies
/// outside the diurnal band (5 to 22 degrees per hour).
/// @note The latitude dependence of Love numbers and anelasticity are ignored.
/// @cite Mathews, P. M., and Buffet, B. A. and Shapiro, I., I., Love numbers
/// Love numbers for a rotating spheroidal Earth∷ New definitions and numerical
/// values, J. Geophys. Res., 22, 579-582, 1995.
constexpr auto love_pmm95b(double frequency) noexcept
    -> std::tuple<double, double, double> {
  if (frequency < 5.0) {
    return {0.299, 0.606, 0.0840};
  }

  if (frequency > 22.0) {
    return {0.302, 0.609, 0.0852};
  }

  constexpr auto f_fcn =
      1.0023214;  // cycles per sidereal day from Mathews Table 1.
  constexpr auto f_k1 = 15.041068;  // reference frequency
  constexpr auto f_o1 = 13.943036;  // reference frequency for O1
  constexpr auto frequency_ratio = f_o1 / f_k1;

  const auto f = frequency / f_k1;
  const auto frac = (f - frequency_ratio) / (f_fcn - f);

  return {0.2962 - 0.00127 * frac, 0.5994 - 0.002532 * frac,
          0.08378 + 0.00007932 * frac};
}

}  // namespace perth
