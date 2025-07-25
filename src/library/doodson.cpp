
#include "perth/doodson.hpp"

#include "perth/datetime.hpp"

namespace perth {
auto calculate_celestial_vector(double time, double delta) noexcept
    -> Vector6d {
  {
    // Terrestrial Time
    auto time_tt = time + delta / static_cast<double>(kSecondsPerDay);

    // Julian Day since J2000
    auto tjd = time_tt + kModifiedJulianEpoch;
    // TT since J2000, in centuries
    auto tx = (tjd - static_cast<double>(kJ2000JulianDay)) /
              static_cast<double>(kDaysPerCentury);

    // Fundamental arguments
    auto args = fundarg(tx);

    // Mean longitude of the Moon
    auto s = args.f + args.om;

    // Mean longitude of the Sun
    auto h = args.f + args.om - args.d;

    // Longitude of lunar perigee
    auto p = args.f + args.om - args.l;

    // Negative longitude of lunar node
    auto fn = -args.om;

    // Longitude of solar perigee
    auto ps = -args.lp + args.f - args.d + args.om;

    // Local sidereal time in radians
    auto tsolar = (time - static_cast<int>(time)) * two_pi<double>();

    // Doodson's tau variable
    auto tau = tsolar - s + h;

    Vector6d vector;
    vector << tau, s, h, p, fn, ps;
    return vector.unaryExpr(
        [](double x) { return normalize_angle(degrees<double>(x)); });
  }
}
}  // namespace perth
