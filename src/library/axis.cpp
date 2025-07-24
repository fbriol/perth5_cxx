#include "perth/axis.hpp"

namespace perth {

auto Axis::normalize_longitude(const Eigen::VectorXd &points)
    -> std::unique_ptr<Eigen::VectorXd> {
  auto monotonic = true;
  auto ascending = points.size() < 2 ? true : points[0] < points[1];

  for (int64_t ix = 1; ix < points.size(); ++ix) {
    monotonic =
        ascending ? points[ix - 1] < points[ix] : points[ix - 1] > points[ix];

    if (!monotonic) {
      break;
    }
  }

  if (!monotonic) {
    auto result = Eigen::VectorXd(points);
    auto cross = false;

    auto *it_current = result.data() + 1;
    auto *it_previous = result.data();
    auto *it_end = result.data() + result.size();
    while (it_current != it_end) {
      if (!cross) {
        cross =
            ascending ? *it_previous > *it_current : *it_previous < *it_current;
      }

      if (cross) {
        *it_current += ascending ? 360.0 : -360.0;
      }
      ++it_current;
      ++it_previous;
    }
    return std::make_unique<Eigen::VectorXd>(std::move(result));
  }
  return nullptr;
}

/// Check if the values in the vector are evenly spaced.
constexpr auto is_evenly_spaced(const Eigen::Ref<const Eigen::VectorXd> &points,
                                const double epsilon) -> std::optional<double> {
  auto n = points.size();

  // The axis is defined by a single value.
  if (n < 2) {
    return {};
  }

  auto increment =
      (points[points.size() - 1] - points[0]) / static_cast<double>(n - 1);

  // If the first two values are constant, the values are not evenly spaced.
  if (std::abs(increment) <= epsilon) {
    return {};
  }

  for (int64_t ix = 1; ix < n; ++ix) {
    if (!is_same(points[ix] - points[ix - 1], increment, epsilon)) {
      return {};
    }
  }
  return increment;
}

auto Axis::initialize(const Eigen::Ref<const Eigen::VectorXd> &values,
                      const double epsilon) -> void {
  // Determines whether the set of data provided can be represented as an
  // interval.
  auto increment = is_evenly_spaced(values, epsilon);
  if (!increment) {
    throw std::invalid_argument(
        "the axis values must be evenly spaced from each other.");
  }
  auto stop = values[values.size() - 1];

  start_ = values[0];
  size_ = values.size();
  step_ = size_ == 1 ? stop - start_
                     : (stop - start_) / static_cast<double>(size_ - 1);

  is_ascending_ = size_ < 2 ? true : (*this)(0) < (*this)(1);

  if (is_periodic_) {
    is_periodic_ =
        is_same(std::abs(step_ * static_cast<double>(size_)), 360.0, epsilon);
  }
}

auto Axis::find_indices(double coordinate) const
    -> std::optional<std::tuple<int64_t, int64_t>> {
  coordinate = normalize_coordinate(coordinate);

  auto length = size_;
  auto i0 = find_index(coordinate, false);

  /// If the value is outside the circle, then the value is between the last
  /// and first index.
  if (i0 == -1) {
    return is_periodic_ ? std::make_optional(std::make_tuple(
                              static_cast<int64_t>(length - 1), 0LL))
                        : std::nullopt;
  }

  // Given the delta between the found coordinate and the given coordinate,
  // chose the other index that frames the coordinate
  auto delta = coordinate - (*this)(i0);
  auto i1 = i0;
  if (delta == 0) {
    // The requested coordinate is located on an element of the axis.
    i1 == length - 1 ? --i0 : ++i1;
  } else {
    if (delta < 0) {
      // The found point is located after the coordinate provided.
      is_ascending_ ? --i0 : ++i0;
      if (is_periodic_) {
        i0 = remainder(i0, length);
      }
    } else {
      // The found point is located before the coordinate provided.
      is_ascending_ ? ++i1 : --i1;
      if (is_periodic_) {
        i1 = remainder(i1, length);
      }
    }
  }
  if (i0 >= 0 && i0 < length && i1 >= 0 && i1 < length) {
    return std::make_tuple(i0, i1);
  }
  return std::nullopt;
}

}  // namespace perth
