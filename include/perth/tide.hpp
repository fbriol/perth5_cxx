#pragma once

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

  auto evaluate_tide(const double lon, const double lat, const double time,
                     ConstituentTable& table, TideTable& tide_table,
                     Inference* inference, Accelerator* acc) const -> Quality;
};

template <typename T>
auto Perth<T>::evaluate_tide(const double lon, const double lat,
                             const double time, ConstituentTable& table,
                             TideTable& tide_table, Inference* inference,
                             Accelerator* acc) const -> Quality {
  // Interpolation, at the requested position, of the waves provided by the
  // model used.
  tidal_model_.interpolate(lon, lat, table, acc);

  if (inference) {
    // If an inference is provided, use it to fill the tide table.
    (*inference)(tide_table);
  }

  // Compute nodal corrections.
  const auto args = acc->update_args(time);
  const auto perigee = args(3);
  const auto omega = -args(4);
  compute_nodal_corrections(omega, perigee, table.retrieve_keys_as_vector());

  for (auto& key : table.keys()) {
    auto& h = tide_table[key];
    auto args = acc->doodson_argument(table[key].first)
  }
}

}  // namespace perth
