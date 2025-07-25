#pragma once

#include <optional>
#include <tuple>

#include "perth/constituent.hpp"
#include "perth/datetime.hpp"
#include "perth/doodson.hpp"
#include "perth/inference.hpp"
#include "perth/nodal_corrections.hpp"
#include "perth/tidal_model.hpp"

namespace perth {

template <typename T>
class Perth {
 public:
  Perth(std::shared_ptr<TidalModel<T>> tidal_model,
        const bool group_modulations = false)
      : tidal_model_(std::move(tidal_model)),
        group_modulations_(group_modulations) {}

  /// @brief Evaluate the tide at the given longitude, latitude, and time.
  /// @param[in] lon Longitudes in degrees.
  /// @param[in] lat Latitudes in degrees.
  /// @param[in] time Time in seconds since the epoch.
  /// @param[in] time_tolerance Tolerance for time matching in seconds.
  /// @param[in] interpolation_type Type of interpolation to use to compute
  /// inferred constituents. If equal to std::nullopt, no inference is done.
  auto evaluate(const Eigen::Ref<const Eigen::VectorXd>& lon,
                const Eigen::Ref<const Eigen::VectorXd>& lat,
                const Eigen::Ref<const Eigen::Vector<int64_t, -1>>& time,
                const double time_tolerance = 0,
                const std::optional<InterpolationType>& interpolation_type =
                    std::nullopt) const
      -> std::tuple<Eigen::VectorXd, Eigen::VectorXd,
                    Eigen::Vector<int8_t, -1>>;

  constexpr auto tidal_model() const -> const std::shared_ptr<TidalModel<T>>& {
    return tidal_model_;
  }

 private:
  std::shared_ptr<TidalModel<T>> tidal_model_;
  bool group_modulations_{false};  ///< Whether to apply group modulations.

  auto evaluate_tide(const double lon, const double lat, const double time,
                     TideTable& tide_table, Inference* inference,
                     Accelerator* acc) const
      -> std::tuple<double, double, Quality>;
};

template <typename T>
auto Perth<T>::evaluate_tide(const double lon, const double lat,
                             const double time, TideTable& tide_table,
                             Inference* inference, Accelerator* acc) const
    -> std::tuple<double, double, Quality> {
  // Interpolation, at the requested position, of the waves provided by the
  // model used.
  auto quality = tidal_model_->interpolate(lon, lat, tide_table, acc);
  if (quality == Quality::kUndefined) {
    // If the interpolation failed, return undefined values.
    return {std::numeric_limits<double>::quiet_NaN(),
            std::numeric_limits<double>::quiet_NaN(), quality};
  }

  if (inference) {
    // If an inference is provided, use it to fill the tide table.
    (*inference)(tide_table, lat);
  }

  // Update astronomical arguments, nodal corrections, and Doodson arguments
  // for tidal constituents if the time has changed significantly.
  acc->update_args(time, group_modulations_, tide_table);

  double tide = 0.0;
  double tide_lp = 0.0;
  // Sum over all constituents to compute the final tide.
  for (size_t ix = 0; ix < tide_table.size(); ++ix) {
    const auto& nodal_correction = acc->nodal_corrections()[ix];
    const auto& component = tide_table.at(ix).second;
    const auto& constituent_tide = component.tide;
    auto x = radians(component.tidal_argument + nodal_correction.u);
    auto h = constituent_tide.real() * nodal_correction.f * std::cos(x) +
             constituent_tide.imag() * nodal_correction.f * std::sin(x);
    if (component.type == ConstituentType::kLongPeriod) {
      tide_lp += h;
      printf("%.12f %.12f %.12f %.12f %.12f %.12f %.12f\n",
             constituent_tide.real(), constituent_tide.imag(),
             nodal_correction.f, nodal_correction.u, component.tidal_argument,
             h, tide_lp);
    } else {
      tide += h;
    }
  }
  return {tide, tide_lp, quality};
}

template <typename T>
auto Perth<T>::evaluate(
    const Eigen::Ref<const Eigen::VectorXd>& lon,
    const Eigen::Ref<const Eigen::VectorXd>& lat,
    const Eigen::Ref<const Eigen::Vector<int64_t, -1>>& time,
    const double time_tolerance,
    const std::optional<InterpolationType>& interpolation_type) const
    -> std::tuple<Eigen::VectorXd, Eigen::VectorXd, Eigen::Vector<int8_t, -1>> {
  auto size = lon.size();
  // Check that the input vectors have the same size.
  if (size != lat.size() || size != time.size()) {
    throw std::invalid_argument("Input vectors must have the same size.");
  }
  // Create the output vectors.
  Eigen::VectorXd tide = Eigen::VectorXd::Zero(size);
  Eigen::VectorXd tide_lp = Eigen::VectorXd::Zero(size);
  Eigen::Vector<int8_t, -1> quality = Eigen::Vector<int8_t, -1>::Zero(size);

  // Create the tide table and accelerator.
  TideTable tide_table = make_tide_table(tidal_model_->identifiers());
  Accelerator acc(time_tolerance, tide_table.size());

  auto inference = std::unique_ptr<Inference>(
      interpolation_type.has_value()
          ? new Inference(tide_table, *interpolation_type)
          : nullptr);
  auto inference_ptr = inference.get();

  for (auto ix = 0; ix < size; ++ix) {
    // Evaluate the tide at the current position and time.
    auto [tide_value, tide_lp_value, quality_value] =
        evaluate_tide(lon(ix), lat(ix), epoch_to_modified_julian_date(time(ix)),
                      tide_table, inference_ptr, &acc);

    // Store the results in the output vectors.
    tide(ix) = tide_value;
    tide_lp(ix) = tide_lp_value;
    quality(ix) = static_cast<int8_t>(quality_value);
  }
  return {tide, tide_lp, quality};
}

}  // namespace perth
