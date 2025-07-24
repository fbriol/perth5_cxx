#pragma once

#include <tuple>

#include "perth/constituent.hpp"
#include "perth/doodson.hpp"
#include "perth/inference.hpp"
#include "perth/nodal_corrections.hpp"
#include "perth/tidal_model.hpp"

namespace perth {

template <typename T>
class Perth {
 public:
 private:
  TidalModel<T> tidal_model_;
  std::vector<Constituent> constituents_;
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
  auto quality = tidal_model_.interpolate(lon, lat, tide_table, acc);
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
  acc->update_args(time, group_modulations_, constituents_, tide_table);

  double tide = 0.0;
  double tide_lp = 0.0;
  // Sum over all constituents to compute the final tide.
  for (size_t ix = 0; ix < tide_table.size(); ++ix) {
    const auto& nodal_correction = acc->nodal_corrections()[ix];
    const auto& component = tide_table.at(ix).second;
    const auto& constituent_tide = component.tide;
    auto x = radians(component.tidal_argument + nodal_correction.u);
    auto h = constituent_tide.real() * nodal_correction.f * std::cos(x) -
             constituent_tide.imag() * nodal_correction.f * std::sin(x);
    if (component.type == ConstituentType::kLongPeriod) {
      tide_lp += h;
    } else {
      tide += h;
    }
  }
  return {tide, tide_lp, quality};
}

}  // namespace perth
