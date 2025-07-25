#include "perth/tidal_model.hpp"

#include "perth/datetime.hpp"

namespace perth {

auto Accelerator::update_args(const double time, const double group_modulations,
                              TideTable& table) -> void {
  if (std::abs(time - time_) < time_tolerance_) {
    return;
  }

  time_ = time;
  delta_ = calculate_delta_time(time + kModifiedJulianEpoch);

  auto args = calculate_celestial_vector(time, delta_);
  const auto perigee = args(3);
  const auto omega = -args(4);
  if (group_modulations) {
    const auto hsolar = args(2);
    const auto psolar = args(5);
    nodal_corrections_ = std::move(compute_nodal_corrections(
        psolar, omega, perigee, hsolar, table.keys_vector()));
  } else {
    nodal_corrections_ = std::move(
        compute_nodal_corrections(omega, perigee, table.keys_vector()));
  }
  for (auto& key : table.keys()) {
    auto& component = table[key];
    component.tidal_argument = calculate_doodson_argument(
        time_, delta_, component.doodson_number.cast<double>());
  }
}

}  // namespace perth
