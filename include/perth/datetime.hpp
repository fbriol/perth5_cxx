#pragma once

namespace perth {

/// @brief The Modified Julian Date epoch in seconds.
constexpr double kModifiedJulianEpoch = 2400000.5;

/// @brief The number of seconds in a day.
constexpr int64_t kSecondsPerDay = 86400;

/// @brief The number of microseconds in a second.
constexpr int64_t kMicrosecondsPerSecond = 1000000;

/// @brief The number of days in a century.
constexpr int64_t kDaysPerCentury = 36525;

/// @brief Julian Day Number for the J2000.0 epoch.
constexpr int64_t kJ2000JulianDay = 2451545;

/// @brief Converts a number of microseconds elapsed since the epoch
/// (1970-01-01) to a Modified Julian Date (MJD).
/// @param epoch The number of microseconds since the epoch (1970-01-01).
/// @return The Modified Julian Date corresponding to the epoch.
constexpr auto epoch_to_modified_julian_date(const int64_t epoch) -> double {
  // 1 MJD = 86400 seconds, and MJD starts at 1858-11-17
  // 1970-01-01 is 40587 days after 1858-11-17
  auto days = epoch / kMicrosecondsPerSecond / kSecondsPerDay;
  auto fractional_mcsec = epoch % (kMicrosecondsPerSecond * kSecondsPerDay);
  return days + (fractional_mcsec / (kMicrosecondsPerSecond * kSecondsPerDay)) +
         40587.0;
}

}  // namespace perth
