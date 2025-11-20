#pragma once

#include <complex>
#include <cstdint>
#include <limits>
#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <vector>

#include "perth/axis.hpp"
#include "perth/constituent.hpp"
#include "perth/grid.hpp"
#include "perth/math.hpp"
#include "perth/nodal_corrections.hpp"

namespace perth {

/// @brief Indicate the quality of the interpolation
enum Quality : uint8_t {
  kExtrapolated1 = 1,  //!< Value extrapolated with one data point
  kExtrapolated2 = 2,  //!< Value extrapolated with two data points
  kExtrapolated3 = 3,  //!< Value extrapolated with three data points
  kInterpolated = 4,   //!< Value correctly interpolated
  kUndefined = 0,      //!< Value undefined
};

/// @brief Constituent values type
using ConstituentValues =
    std::vector<std::pair<Constituent, std::complex<double>>>;

class Accelerator {
 public:
  Accelerator(const double time_tolerance, const size_t n_constituents)
      : time_tolerance_(time_tolerance) {
    values_.reserve(n_constituents);
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

  constexpr auto x1() const noexcept -> double { return x1_; }
  constexpr auto x2() const noexcept -> double { return x2_; }
  constexpr auto y1() const noexcept -> double { return y1_; }
  constexpr auto y2() const noexcept -> double { return y2_; }

  constexpr auto time_tolerance() const noexcept -> double {
    return time_tolerance_;
  }

  constexpr auto size() const noexcept -> size_t { return values_.capacity(); }

  /// @brief Returns the tidal constituent values interpolated at the given
  /// point.
  /// @return the tidal constituent values interpolated at the given point.
  constexpr auto values() const noexcept -> const ConstituentValues& {
    return values_;
  }

  constexpr auto nodal_corrections() const noexcept
      -> const std::vector<NodalCorrections>& {
    return nodal_corrections_;
  }

  /// @brief Clears the cached interpolated values.
  inline auto clear() noexcept -> void { values_.clear(); }

  /// @brief Appends a tidal constituent value to the cached interpolated
  /// values.
  auto emplace_back(const Constituent& constituent,
                    const std::complex<double>& value) noexcept -> void {
    values_.emplace_back(constituent, value);
  }

  auto update_args(const double time, const double group_modulations,
                   ConstituentTable& constituent_table) -> void;

 private:
  /// @brief Time in seconds for which astronomical angles are considered
  /// constant
  double time_tolerance_;

  /// @brief The time used to compute the celestial vector.
  double time_{std::numeric_limits<double>::max()};

  /// @brief Latest delta time (TT - UT) used for celestial calculations.
  double delta_{std::numeric_limits<double>::max()};

  /// @brief The tidal constituent values interpolated at the last point.
  ConstituentValues values_;

  /// @brief The latest nodal corrections computed.
  std::vector<NodalCorrections> nodal_corrections_;

  double x1_{std::numeric_limits<double>::max()};
  double x2_{std::numeric_limits<double>::max()};
  double y1_{std::numeric_limits<double>::max()};
  double y2_{std::numeric_limits<double>::max()};
};

template <typename T>
class TidalModel : public std::enable_shared_from_this<TidalModel<T>> {
 public:
  /// @brief Construct a tidal model with longitude and latitude axes.
  /// @param lon Longitude axis (will be moved).
  /// @param lat Latitude axis (will be moved).
  /// @param row_major Whether the data is stored in longitude-major order.
  /// @throw std::exception Strong exception safety guarantee.
  TidalModel(Axis lon, Axis lat, const bool row_major = true)
      : data_(),
        lon_(std::move(lon)),
        lat_(std::move(lat)),
        row_major_(row_major) {}

  [[nodiscard]] auto accelerator(const double time_tolerance) const
      -> std::unique_ptr<Accelerator> {
    return std::make_unique<Accelerator>(time_tolerance, this->data_.size());
  }

  inline auto add_constituent(
      const Constituent ident,
      const Eigen::Ref<
          const Eigen::Matrix<std::complex<T>, -1, -1, Eigen::RowMajor>>& wave)
      -> void {
    auto row_major = wave.rows() == lon_.size() && wave.cols() == lat_.size();
    if (row_major != row_major_) {
      throw std::invalid_argument(
          "The data is not in the expected row-major order: expected " +
          std::string(row_major_ ? "true" : "false") + ", got " +
          std::string(row_major ? "true" : "false"));
    }
    if (row_major) {
      if (wave.rows() != lon_.size() || wave.cols() != lat_.size()) {
        throw std::invalid_argument(
            "The data size does not match the axes size: expected (" +
            std::to_string(lon_.size()) + "x" + std::to_string(lat_.size()) +
            "), got (" + std::to_string(wave.rows()) + "x" +
            std::to_string(wave.cols()) + ")");
      }
    } else {
      if (wave.rows() != lat_.size() || wave.cols() != lon_.size()) {
        throw std::invalid_argument(
            "The data size does not match the axes size: expected (" +
            std::to_string(lat_.size()) + "x" + std::to_string(lon_.size()) +
            "), got (" + std::to_string(wave.rows()) + "x" +
            std::to_string(wave.cols()) + ")");
      }
    }
    this->data_.emplace(ident,
                        Eigen::Map<const Eigen::Vector<std::complex<T>, -1>>(
                            wave.data(), wave.size()));
  }

  inline auto interpolate(const double lon, const double lat,
                          ConstituentTable& constituent_table,
                          Accelerator* acc) const -> Quality {
    Quality quality;
    for (const auto& item : this->interpolate(lon, lat, quality, acc)) {
      constituent_table[item.first].tide = std::move(item.second);
    }
    return quality;
  }

  /// True if no tidal constituent is handled by the model.
  [[nodiscard]] auto empty() const -> bool { return data_.empty(); }

  /// Get the number of tidal constituents handled by the model.
  [[nodiscard]] auto size() const -> size_t { return data_.size(); }

  /// Get the tidal constituent identifiers handled by the model.
  [[nodiscard]] inline auto identifiers() const -> std::vector<Constituent> {
    auto result = std::vector<Constituent>();
    result.reserve(data_.size());
    for (const auto& item : data_) {
      result.push_back(item.first);
    }
    return result;
  }

 private:
  /// The constituents of the tidal model.
  std::unordered_map<Constituent, Eigen::Vector<std::complex<T>, -1>> data_;
  /// Longitude axis.
  const Axis lon_;
  /// Latitude axis.
  const Axis lat_;
  /// Whether the data is stored in longitude-major order.
  const bool row_major_;

  auto interpolate(const double lon, const double lat, Quality& quality,
                   Accelerator* acc) const -> const ConstituentValues&;
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
    for (const auto& item : this->data_) {
      acc->emplace_back(item.first, undefined_value);
    }
    quality = Quality::kUndefined;
    return acc->values();
  };

  // Find the nearest point in the grid
  auto lon_index = lon_.find_indices(lon);
  auto lat_index = lat_.find_indices(lat);

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

  if (x1 == acc->x1() && x2 == acc->x2() && y1 == acc->y1() &&
      y2 == acc->y2()) {
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
  quality = (n == static_cast<int64_t>(Quality::kInterpolated))
                ? Quality::kInterpolated
            : (n == static_cast<int64_t>(Quality::kExtrapolated3))
                ? Quality::kExtrapolated3
            : (n == static_cast<int64_t>(Quality::kExtrapolated2))
                ? Quality::kExtrapolated2
            : (n == static_cast<int64_t>(Quality::kExtrapolated1))
                ? Quality::kExtrapolated1
                : Quality::kUndefined;
  return acc->values();
}

}  // namespace perth
