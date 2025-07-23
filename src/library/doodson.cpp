#include "perth/doodson.hpp"

namespace perth {
auto calculate_celestial_vector(double time, double delta) noexcept
    -> Vector6d {
  {
    // Terrestrial Time
    auto time_tt = time + delta / 86400.0;

    // Julian Day since J2000
    auto tjd = time_tt + 2400000.5;
    auto tx = (tjd - 2451545.0) / 36525.0;  // TT since J2000, in centuries

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
