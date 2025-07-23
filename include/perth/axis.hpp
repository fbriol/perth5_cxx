#pragma once

#include <Eigen/Core>
#include <limits>
#include <memory>
#include <optional>
#include <stdexcept>
#include <tuple>

#include "perth/eigen.hpp"
#include "perth/math.hpp"

namespace perth {

/// @brief Represents an axis, which is a mathematical object that relates the
/// coordinate positions in a vector to the physical values represented by the
/// axis.
///
/// An axis can be used to represent various physical quantities, such as
/// longitude, latitude, temperature, etc. For example, an axis representing
/// longitude can have 360 values, ranging from -180 to 179 degrees. The
/// coordinate positions in the axis are the indices of the values in the axis.
class Axis {
 public:
  /// Default constructor.
  Axis() = default;

  /// Build an axis from a vector of points.
  ///
  /// @param[in] points The axis points.
  /// @param[in] epsilon The tolerance used to determine if the axis is
  /// periodic.
  /// @param[in] is_periodic If true, the axis is periodic (i.e., the first and
  /// last points are connected).
  explicit Axis(const Eigen::Ref<const Eigen::VectorXd> &points,
                const double epsilon = 1e-6, const bool is_periodic = false)
      : is_periodic_(is_periodic) {
    if (points.size() > std::numeric_limits<int64_t>::max()) {
      throw std::invalid_argument(
          "the size of the axis must not contain more than " +
          std::to_string(std::numeric_limits<int64_t>::max()) + "elements.");
    }

    if (points.size() < 2) {
      throw std::invalid_argument(
          "the size of the axis must contain at least 2 elements.");
    }
    // If this is a periodic axis, normalize the points.
    if (is_periodic_) {
      auto normalized_values = Axis::normalize_longitude(points);
      normalized_values ? initialize(*normalized_values, epsilon)
                        : initialize(points, epsilon);
    } else {
      initialize(points);
    }
  }

  /// Build an axis from a start, end, and step.
  ///
  /// @param[in] start The first value of the axis.
  /// @param[in] end The last value of the axis.
  /// @param[in] step The step between two values of the axis.
  /// @param[in] epsilon The tolerance used to determine if the axis is
  /// periodic.
  /// @param[in] is_periodic If true, the axis is periodic (i.e., the first and
  /// last points are connected).
  Axis(const double start, const double end, const double step,
       const double epsilon = 1e-6, const bool is_periodic = false)
      : Axis(Eigen::VectorXd::LinSpaced(
                 static_cast<int>((end - start) / step) + 1, start, end),
             epsilon, is_periodic) {}

  /// Compare two axes for equality.
  ///
  /// @param[in] other The other axis.
  /// @return True if the axes are equal.
  constexpr auto operator==(const Axis &other) const noexcept -> bool {
    return is_periodic_ == other.is_periodic_ &&
           is_ascending_ == other.is_ascending_ && size_ == other.size_ &&
           start_ == other.start_ && step_ == other.step_;
  }

  /// @brief Test if two axes are different.
  ///
  /// @param[in] other The other axis.
  /// @return True if the axes are different.
  constexpr auto operator!=(const Axis &other) const noexcept -> bool {
    return is_periodic_ != other.is_periodic_ ||
           is_ascending_ != other.is_ascending_ || size_ != other.size_ ||
           start_ != other.start_ || step_ != other.step_;
  }

  /// Return the size of the axis.
  constexpr auto size() const noexcept -> int64_t { return size_; }

  /// Return the first value of the axis.
  constexpr auto start() const noexcept -> double { return start_; }

  /// Return the last value of the axis.
  constexpr auto end() const -> double { return (*this)(size() - 1); }

  /// Return the step of the axis.
  constexpr auto step() const noexcept -> double { return step_; }

  /// Return the minimum value of the axis.
  constexpr auto min_value() const -> double {
    return is_ascending_ ? start() : end();
  }

  /// Return the maximum value of the axis.
  constexpr auto max_value() const -> double {
    return is_ascending_ ? end() : start();
  }

  /// Clamp the value to the axis.
  ///
  /// @param[in] value The value to clamp.
  /// @return The clamped value.
  constexpr auto clamp(const double value) const -> double {
    return std::clamp(value, min_value(), max_value());
  }

  /// True if the axis is ascending.
  constexpr auto is_ascending() const -> bool { return is_ascending_; }

  /// True if the axis is periodic.
  constexpr auto is_periodic() const -> bool { return is_periodic_; }

  /// Return the value at the given index.
  ///
  /// @param[in] index The index.
  /// @return The value at the given index.
  constexpr auto operator()(const int64_t index) const -> double {
    if (index < 0 || index >= size_) {
      throw std::out_of_range("The index is out of range.");
    }
    return static_cast<double>(start_ + static_cast<double>(index) * step_);
  }

  /// Search the index on the axis that is closest to the given value.
  ///
  /// @param[in] coordinate The coordinate to search.
  /// @param[in] bounded True if the search should be bounded by the
  /// axis limits. If false, return 0 if the value is smaller than the
  /// axis start and size() - 1 if the value is larger than the axis
  /// end.
  /// @return The index of the closest value. Return -1 if the value is
  /// out of bounds and bounded is false.
  PERTH_MATH_CONSTEXPR auto find_index(
      const double coordinate, const bool bounded = false) const noexcept
      -> int64_t {
    auto index = static_cast<int64_t>(
        std::round((normalize_coordinate(coordinate) - start_) / step_));
    if (index < 0) {
      return bounded ? 0 : -1;
    }
    if (index >= this->size_) {
      return bounded ? this->size_ - 1 : -1;
    }
    return index;
  }

  /// @brief Given a coordinate position, find grids elements around it.
  /// This mean that
  /// @code
  /// (*this)(i0) <= coordinate < (*this)(i1)
  /// @endcode
  ///
  /// @param[in] coordinate position in this coordinate system
  /// @return None if coordinate is outside the axis definition domain otherwise
  /// the tuple (i0, i1)
  auto find_indices(double coordinate) const
      -> std::optional<std::tuple<int64_t, int64_t>>;

  /// @brief Given a coordinate position, find grids elements around it.
  /// This mean that
  /// @code
  /// (*this)(i0) <= coordinate < (*this)(i1)
  /// @endcode
  /// If the coordinate is outside the axis definition domain, the closest
  /// elements are returned, i.e., the first or last element of the axis.
  ///
  /// @param[in] coordinate position in this coordinate system
  /// @return The tuple (i0, i1)
  inline auto find_bounded_indices(double coordinate) const
      -> std::tuple<int64_t, int64_t> {
    auto indices = find_indices(coordinate);
    if (indices.has_value()) {
      return *indices;
    }
    if (size() == 1) {
      return {0, 0};
    }
    auto ix0 = find_index(coordinate, true);
    if (ix0 == 0) {
      return {0, 1};
    }
    return {ix0 - 1, ix0};
  }

 private:
  /// True if the axis is periodic.
  bool is_periodic_ = false;
  /// True if the axis is ascending.
  bool is_ascending_{};
  /// The size of the axis.
  int64_t size_{};
  /// The first value of the axis.
  double start_{};
  /// The step between two values of the axis.
  double step_{};

  /// Put longitude into the range [0, circle_] degrees.
  static auto normalize_longitude(const Eigen::VectorXd &points)
      -> std::unique_ptr<Eigen::VectorXd>;

  /// Initializes the axis container from values.
  auto initialize(const Eigen::Ref<const Eigen::VectorXd> &values,
                  const double epsilon = 1e-6) -> void;

  /// Returns the normalized value of the coordinate with the respect to the
  /// axis definition.
  ///
  /// @param[in] coordinate The coordinate to be normalized.
  /// @return The normalized value of the coordinate.
  constexpr auto normalize_coordinate(const double coordinate) const -> double {
    if (is_periodic_ &&
        (coordinate >= min_value() + 360.0 || coordinate < min_value())) {
      return normalize_angle(coordinate, min_value(), 360.0);
    }
    return coordinate;
  }
};

}  // namespace perth
