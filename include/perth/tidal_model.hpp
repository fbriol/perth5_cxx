#pragma once

#include <complex>
#include <cstdint>
#include <limits>
#include <stdexcept>
#include <unordered_map>
#include <vector>

#include "perth/axis.hpp"
#include "perth/constituent.hpp"
#include "perth/delta_t.hpp"
#include "perth/doodson.hpp"
#include "perth/grid.hpp"
#include "perth/math.hpp"

namespace perth {

/// @brief Indicate the quality of the interpolation
enum Quality : uint8_t {
  kExtrapolated1 = 1,  //!< Value extrapolated with one data point
  kExtrapolated2 = 2,  //!< Value extrapolated with two data points
  kExtrapolated3 = 3,  //!< Value extrapolated with three data points
  kInterpolated = 4,   //!< Value correctly interpolated
  kUndefined = 0,      //!< Value undefined
};

/// @brief Possible tide types
enum TideType : uint8_t {
  kTide = 0x01,    //!< Ocean Tide
  kRadial = 0x02,  //!< Radial tide
};

/// @brief Constituent values type
using ConstituentValues =
    std::vector<std::pair<Constituent, std::complex<double>>>;

struct CelestialArguments {
  /// @brief The time in seconds for which the arguments are valid.
  double time;
  /// @brief The delta time in seconds used to compute the celestial vector.
  double delta;
  /// @brief The celestial vector used to evaluate the tidal constituents.
  Vector6d celestial_vector;
};

class Accelerator {
 public:
  Accelerator(const double time_tolerance, const size_t n_constituents)
      : time_tolerance_(time_tolerance),
        args_{std::numeric_limits<double>::max(), {}} {
    values_.reserve(n_constituents);
  }

  auto doodson_argument(const Eigen::Ref<const Vector6d>& numbers) const
      -> double {
    auto beta = calculate_celestial_vector(args_.time, args_.delta);
    auto arg = beta.dot(numbers);
    return std::fmod(arg, 360.0);
  }

  /// @brief Casts the object to a pointer of type T.
  ///
  /// @tparam T The type to cast to.
  /// @return A pointer to the object casted to type T, or nullptr if the cast
  /// is not possible.
  template <typename T>
  constexpr auto cast() noexcept -> T* {
    auto* type_info = &typeid(*this);
    if (*type_info != typeid(T)) {
      return nullptr;
    }
    return reinterpret_cast<T*>(this);
  }

  /// @brief Returns the tidal constituent values interpolated at the given
  /// point.
  /// @return the tidal constituent values interpolated at the given point.
  constexpr auto values() const noexcept -> const ConstituentValues& {
    return values_;
  }

  /// @brief Clears the cached interpolated values.
  inline auto clear() noexcept -> void { values_.clear(); }

  /// @brief Appends a tidal constituent value to the cached interpolated
  /// values.
  auto emplace_back(const Constituent& constituent,
                    const std::complex<double>& value) noexcept -> void {
    values_.emplace_back(constituent, value);
  }

  auto update_args(const double time) -> const Vector6d& {
    if (std::abs(time - args_.time) > time_tolerance_) {
      args_.delta = calculate_delta_time(time);
      args_.time = time;
      args_.celestial_vector = calculate_celestial_vector(time, delta_);
    }
    return args_.celestial_vector;
  }

 private:
  /// @brief Time in seconds for which astronomical angles are considered
  /// constant
  double time_tolerance_;

  /// @brief The last angle used to evaluate the tidal constituents.
  CelestialArguments args_;

  /// @brief The tidal constituent values interpolated at the last point.
  ConstituentValues values_;

  double delta_{std::numeric_limits<double>::max()};

  double x1_{std::numeric_limits<double>::max()};
  double x2_{std::numeric_limits<double>::max()};
  double y1_{std::numeric_limits<double>::max()};
  double y2_{std::numeric_limits<double>::max()};
};

template <typename T>
class TidalModel {
 public:
  TidalModel(Axis lon, Axis lat, const TideType tide_type,
             const bool row_major = true)
      : data_(),
        lon_(std::move(lon)),
        lat_(std::move(lat)),
        row_major_(row_major),
        tide_type_(tide_type) {}

  constexpr auto accelerator(const double time_tolerance) const
      -> Accelerator* {
    return new Accelerator(time_tolerance, this->data_.size());
  }

  inline auto add_constituent(const Constituent ident,
                              Vector<std::complex<T>> wave) -> void {
    if (wave.size() != lon_.size() * lat_.size()) {
      throw std::invalid_argument("wave size does not match expected size");
    }
    this->data_.emplace(ident, std::move(wave));
  }

  auto interpolate(const double lon, const double lat, Quality& quality,
                   Accelerator* acc) const -> const ConstituentValues&;

  inline auto interpolate(const double lon, const double lat, TideTable& table,
                          Accelerator* acc) const -> Quality {
    Quality quality;
    for (const auto& item : this->interpolate(point, quality, acc)) {
      table[item.first] = std::move(item.second);
    }
    return quality;
  }

  /// True if no tidal constituent is handled by the model.
  constexpr auto empty() const -> bool { return data_.empty(); }

  /// Get the number of tidal constituents handled by the model.
  constexpr auto size() const -> size_t { return data_.size(); }

  /// Get the tidal constituent identifiers handled by the model.
  inline auto identifiers() const -> std::vector<Constituent> {
    auto result = std::vector<Constituent>();
    result.reserve(data_.size());
    for (const auto& item : data_) {
      result.push_back(item.first);
    }
    return result;
  }

  /// Get the tide type handled by the model.
  constexpr auto tide_type() const -> TideType { return tide_type_; }

 private:
  /// The constituents of the tidal model.
  std::unordered_map<Constituent, std::vector<std::complex<T>>> data_;
  /// Longitude axis.
  Axis lon_;
  /// Latitude axis.
  Axis lat_;
  /// Whether the data is stored in longitude-major order.
  bool row_major_;
  /// Tide type
  TideType tide_type_;
};

template <typename T>
inline auto TidalModel<T>::interpolate(const double lon, const double lat,
                                       Quality& quality, Accelerator* acc) const
    -> const ConstituentValues& {
  // Undefined value for the interpolation. This value is used to indicate
  // that the interpolation failed or that the value is not defined.
  constexpr auto undefined_value =
      std::complex<double>(std::numeric_limits<double>::quiet_NaN(),
                           std::numeric_limits<double>::quiet_NaN());

  // Reset the values to undefined if the point is not within the grid or if
  // the model is undefined for the given point.
  auto reset_values_to_undefined = [&]() -> const ConstituentValues& {
    std::numeric_limits<double>::quiet_NaN();

    for (const auto& item : this->data_) {
      acc->emplace_back(item.first, undefined_value);
    }
    quality = Quality::kUndefined;
    return acc->values();
  };

  // Find the nearest point in the grid
  auto lon_index = lon_.find_indices(point.lon());
  auto lat_index = lat_.find_indices(point.lat());

  if (!lon_index || !lat_index) {
    acc->clear();
    return reset_values_to_undefined();
  }

  // Retrieve the indices of the nearest points in the grid
  auto [i1, i2] = *lon_index;
  auto [j1, j2] = *lat_index;
  const auto x1 = lon_(i1);
  const auto x2 = lon_(i2);
  const auto y1 = lat_(j1);
  const auto y2 = lat_(j2);

  if (x1 == x1_ && x2 == x2_ && y1 == y1_ && y2 == y2_) {
    // If the point is already cached, return the cached values.
    return acc->values();
  }
  // Remove all previous values interpolated.
  acc->clear();

  auto n = int64_t{0};

  // Compute the weights for the bilinear interpolation
  auto wxy = bilinear_weights(normalize_angle(lon, x1), lat, x1, y1,
                              normalize_angle(x2, x1), y2);

  // Create the object that handles the grid data.
  auto grid =
      Grid<std::complex<T>>(nullptr, static_cast<size_t>(lon_.size()),
                            static_cast<size_t>(lat_.size()), row_major_);

  // For each constituent, handled by this model, compute the
  // bilinear interpolation of the value at the given point.
  for (const auto& item : this->data_) {
    grid.data(item.second.data());
    auto value = bilinear_interpolation<std::complex<double>>(
        std::get<0>(wxy), std::get<1>(wxy), std::get<2>(wxy), std::get<3>(wxy),
        grid(i1, j1), grid(i1, j2), grid(i2, j1), grid(i2, j2), n);
    // The computed value lies within the grid boundaries, but it is NaN (not a
    // number).
    if (std::isnan(value.real()) || std::isnan(value.imag())) {
      return reset_values_to_undefined();
    }
    acc->emplace_back(item.first, value);
  }

  // Set the quality of the interpolation based on the number of
  // surrounding points used for the interpolation.
  quality = (n == 4)   ? Quality::kInterpolated
            : (n == 3) ? Quality::kExtrapolated3
            : (n == 2) ? Quality::kExtrapolated2
            : (n == 1) ? Quality::kExtrapolated1
                       : Quality::kUndefined;
  return acc->values();
}

}  // namespace perth
