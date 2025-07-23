#pragma once

#include <Eigen/Core>
#include <complex>
#include <cstdint>

namespace perth {

/// @brief Type alias for a 6-dimensional vector using Eigen library, storing
/// double precision values.
using Vector6d = Eigen::Vector<double, 6>;

/// @brief Type alias for a 6-dimensional vector using Eigen library, storing
/// int8_t precision values.
using Vector6b = Eigen::Vector<int8_t, 6>;

/// @brief Type alias for complex numbers storing double precision values.
using Complex = std::complex<double>;

/// @brief Type alias for a 3-dimensional vector of complex numbers using Eigen
/// library.
using Vector3c = Eigen::Vector3<Complex>;

/// @brief Type alias for a 3-dimensional vector using Eigen library.
using Matrix3d = Eigen::Matrix3d;

}  // namespace perth
