#pragma once

#include <cmath>
#include <complex>
#include <numbers>

#if defined(__GNUC__) && (__GNUC__ >= 11) || \
    defined(__clang__) && (__cplusplus >= 202303L)
#define PERTH_MATH_CONSTEXPR constexpr
#else
#define PERTH_MATH_CONSTEXPR inline
#endif

namespace perth {

/// @brief Constructs a NaN value of type T.
template <typename T>
constexpr auto construct_nan() -> T {
  if constexpr (std::is_floating_point_v<T>) {
    return std::numeric_limits<T>::quiet_NaN();
  } else if constexpr (std::is_same_v<T, std::complex<float>>) {
    return std::complex<float>(std::numeric_limits<float>::quiet_NaN(),
                               std::numeric_limits<float>::quiet_NaN());
  } else if constexpr (std::is_same_v<T, std::complex<double>>) {
    return std::complex<double>(std::numeric_limits<double>::quiet_NaN(),
                                std::numeric_limits<double>::quiet_NaN());
  } else if constexpr (std::is_same_v<T, std::complex<long double>>) {
    return std::complex<long double>(
        std::numeric_limits<long double>::quiet_NaN(),
        std::numeric_limits<long double>::quiet_NaN());
  } else {
    static_assert(std::is_floating_point_v<T> ||
                      std::is_same_v<T, std::complex<float>> ||
                      std::is_same_v<T, std::complex<double>> ||
                      std::is_same_v<T, std::complex<long double>>,
                  "T must be a floating-point or complex type");
  }
}

/// @brief Evaluate x^n
/// @tparam T The type of the number.
/// @tparam N The power to raise the number to.
template <typename T, unsigned N>
struct Power {
  ///  @brief Evaluate x^n
  ///
  /// @param[in] x The number to raise to the power.
  /// @return The result of raising the number to the power.
  static constexpr auto eval(const T& x) -> T {
    return x * Power<T, N - 1>().eval(x);
  }
};

/// @brief Specialization of Power struct for N = 0.
template <typename T>
struct Power<T, 0> {
  /// @brief Evaluate x^0
  ///
  /// @return The result of raising the number to the power (always 1).
  static constexpr auto eval(const T& /*x*/) -> T { return 1; }
};

/// @brief Calculate the power of a number.
///
/// This function calculates the power of a number by using the Power struct.
///
/// @tparam N The power to raise the number to.
/// @tparam T The type of the number.
/// @param[in] x The number to raise to the power.
/// @return The result of raising the number to the power.
template <unsigned N, typename T>
constexpr auto pow(const T& x) -> T {
  return Power<T, N>::eval(x);
}

/// @brief Get the value of π.
///
/// This function returns the value of π (pi).
///
/// @tparam T The type of the return value.
/// @return The value of π.
template <typename T>
constexpr auto pi() noexcept -> T {
  return std::numbers::pi_v<T>;
}

/// @brief Get the value of π/2.
///
/// This function returns the value of π/2 (pi/2).
///
/// @tparam T The type of the return value.
/// @return The value of π/2.
template <typename T>
constexpr auto pi_2() noexcept -> T {
  return 0.5 * pi<T>();
}

/// @brief Get the value of 2π.
///
/// This function returns the value of 2π (2pi).
///
/// @tparam T The type of the return value.
/// @return The value of 2π.
template <typename T>
constexpr auto two_pi() noexcept -> T {
  return T(2) * pi<T>();
}

/// @brief Convert angle x from degrees to radians.
///
/// This function converts an angle from degrees to radians.
///
/// @tparam T The type of the angle.
/// @param[in] x The angle in radians.
/// @return The angle in degrees.
template <typename T>
constexpr auto radians(const T& x) noexcept -> T {
  return x * pi<T>() / T(180);
}

/// @brief Convert angle x from radians to degrees.
///
/// This function converts an angle from radians to degrees.
///
/// @tparam T The type of the angle.
/// @param[in] x The angle in degrees.
/// @return The angle in radians.
template <typename T>
constexpr auto degrees(const T& x) noexcept -> T {
  return x * T(180) / pi<T>();
}

/// Convert angle x from arcseconds to radians.
///
/// @tparam T The type of the result.
/// @param[in] x Angle in arcseconds.
/// @return Angle in radians.
template <typename T>
constexpr auto arcseconds2radians(const T& x) noexcept -> T {
  // 1 arcsecond = 1/3600 degrees
  // 1 degree = π/180 radians
  // => 1 arcsecond = π/(180 * 3600) radians
  return x * pi<T>() / (T(180) * T(3600));
}

/// Computes the remainder of the operation x/y.
///
/// This function has two specializations based on the type `T`:
/// - **Integral Types** (`std::is_integral_v<T>`):
///   - Uses the `%` operator.
///   - The result has the same sign as the denominator `y`.
/// - **Floating-Point Types** (`std::is_floating_point_v<T>`):
///   - Uses `std::remainder`.
///   - If the result's sign differs from `y`, `y` is added to the result to
///     match the sign.
///
/// @tparam T Data type (must be either an integral or floating-point type)
/// @param[in] x Numerator
/// @param[in] y Denominator
/// @return A result with the same sign as its second operand
template <typename T>
constexpr auto remainder(const T& x, const T& y) noexcept -> T {
  if constexpr (std::is_integral_v<T>) {
    auto result = x % y;
    return result != 0 && (result ^ y) < 0 ? result + y : result;
  } else if constexpr (std::is_floating_point_v<T>) {
    auto result = std::remainder(x, y);
    if (std::signbit(result) != std::signbit(y)) {
      result += y;
    }
    return result;
  }
  static_assert(std::is_integral_v<T> || std::is_floating_point_v<T>,
                "T must be either an integral or floating-point type");
}

/// Normalize an angle.
///
/// @param[in] x The angle in degrees.
/// @param[in] min Minimum circle value
/// @param[in] circle Circle value
/// @return the angle reduced to the range [min, circle + min[
template <typename T>
constexpr auto normalize_angle(const T& x, const T& min = T(-180),
                               const T& circle = T(360)) noexcept -> T {
  return remainder(x - min, circle) + min;
}

/// @brief Check if two values are approximately equal within a given epsilon.
///
/// This function checks if two values are approximately equal within a given
/// epsilon. It is specifically designed for floating-point types.
///
/// @tparam T The type of the values.
/// @param[in] a The first value.
/// @param[in] b The second value.
/// @param[in] epsilon The epsilon value for comparison.
/// @return True if the values are approximately equal, false otherwise.
template <typename T,
          std::enable_if_t<std::is_floating_point_v<T>, T>* = nullptr>
constexpr auto is_same(const T& a, const T& b, const T& epsilon) noexcept
    -> bool {
  auto diff = std::fabs(a - b);
  if (diff <= epsilon) {
    return true;
  }
  if (diff < std::fmax(std::fabs(a), std::fabs(b)) * epsilon) {
    return true;
  }
  return false;
}

/// @brief Evaluates a polynomial using Horner's method.
///
/// @tparam T The type of the input and output values.
/// @tparam Args The types of the polynomial coefficients.
/// @param[in] x The input value.
/// @param[in] args The polynomial coefficients.
/// @return The value of the polynomial evaluated at x.
/// @remark This function evaluates the polynomial using Horner's method.
/// The polynomial coefficients are passed as variadic arguments.
/// The first argument is the input value x, followed by the coefficients
/// in decreasing order of degree.
/// For example, to evaluate the polynomial `3x^3 + 2x^2 + x + 1 at x = 2`,
/// call `horner(2, 3, 2, 1, 1)`.
template <typename T, typename... Args>
constexpr auto horner(const T x, Args... args) noexcept -> T {
  static_assert(sizeof...(args) > 0, "At least one coefficient is required.");
  const T coefficients[] = {args...};
  auto ix = sizeof...(args) - 1;
  auto result = coefficients[ix];
  while (ix > 0) {
    ix--;
    result = result * x + coefficients[ix];
  }
  return result;
}

/// Calculate the bilinear weights
///
/// @param[in] x x coordinate
/// @param[in] y y coordinate
/// @param[in] x1 x coordinate of the first point
/// @param[in] y1 y coordinate of the first point
/// @param[in] x2 x coordinate of the second point
/// @param[in] y2 y coordinate of the second point
/// @return a tuple that contains the weights for all four points
template <typename T>
constexpr auto bilinear_weights(const T& x, const T& y, const T& x1,
                                const T& y1, const T& x2, const T& y2) noexcept
    -> std::tuple<T, T, T, T> {
  const auto dx = T(1) / (x2 - x1);
  const auto dy = T(1) / (y2 - y1);
  const auto wx1 = (x2 - x) * dx;
  const auto wx2 = (x - x1) * dx;
  const auto wy1 = (y2 - y) * dy;
  const auto wy2 = (y - y1) * dy;
  return std::make_tuple(wx1, wx2, wy1, wy2);
}

/// Calculate the bilinear interpolation
///
/// @param[in] wx1 weight for the first point in x
/// @param[in] wx2 weight for the second point in x
/// @param[in] wy1 weight for the first point in y
/// @param[in] wy2 weight for the second point in y
/// @param[in] z11 value of the first point
/// @param[in] z12 value of the second point
/// @param[in] z21 value of the third point
/// @param[in] z22 value of the fourth point
/// @param[out] n The number of points used for the interpolation
/// @return the interpolated value
template <typename T>
constexpr auto bilinear_interpolation(const T& wx1, const T& wx2, const T& wy1,
                                      const T& wy2, const T& z11, const T& z12,
                                      const T& z21, const T& z22,
                                      int64_t& n) noexcept -> T {
  auto result = T(0);
  auto sum_w = T(0);
  n = 0;
  auto add_sample = [&result, &sum_w, &n](const T& z, const T& w) {
    if (z - z == T(0)) {
      result += (z * w);
      sum_w += w;
      ++n;
    }
  };
  add_sample(z11, wx1 * wy1);
  add_sample(z12, wx1 * wy2);
  add_sample(z21, wx2 * wy1);
  add_sample(z22, wx2 * wy2);
  return std::abs(sum_w) > 0 ? result / sum_w : construct_nan<T>();
}

}  // namespace perth
