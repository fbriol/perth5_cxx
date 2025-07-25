#pragma once

namespace perth {

/// @brief Converts a number of microseconds elapsed since the epoch
/// (1970-01-01) to a Modified Julian Date (MJD).
/// @param epoch The number of microseconds since the epoch (1970-01-01).
/// @return The Modified Julian Date corresponding to the epoch.
constexpr auto epoch_to_modified_julian_date(const int64_t epoch) -> double {
  // 1 MJD = 86400 seconds, and MJD starts at 1858-11-17
  // 1970-01-01 is 40587 days after 1858-11-17
  auto days = epoch / 86400000000;
  auto fractional_mcsec = epoch % 86400000000;
  return days + (fractional_mcsec / 86400000000.0) + 40587.0;
}

}  // namespace perth
