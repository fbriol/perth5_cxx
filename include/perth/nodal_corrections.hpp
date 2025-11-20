#pragma once

#include <vector>

#include "perth/constituent.hpp"

namespace perth {

/// Result structure for nodal corrections
struct NodalCorrections {
  double f;  ///< Modulation factor
  double u;  ///< Phase correction in degrees
};

/// @brief Computes nodal corrections for a given list of constituents.
/// @param omega Mean longitude of lunar node, in degrees.
/// @param p Mean longitude of lunar perigee, in degrees.
/// @param constituents A vector of constituents for which to compute
/// corrections.
/// @return A vector of NodalCorrections containing the computed corrections for
/// each constituent.
/// @note omega = -N' (where N' is the 5th Doodson number); omega is decreasing
/// in time whereas N' is increasing.
auto compute_nodal_corrections(double omega, double p,
                               const std::vector<Constituent> &constituents)
    -> std::vector<NodalCorrections>;

/// Compute nodal corrections for a single constituent
/// @param omega Mean longitude of lunar node, in degrees
/// @param p Mean longitude of lunar perigee, in degrees
/// @param constituent Single constituent name
/// @return Nodal corrections {f, u}
inline auto compute_nodal_correction(double omega, double p,
                                     const Constituent constituent)
    -> NodalCorrections {
  return compute_nodal_corrections(omega, p, {constituent})[0];
}

/// @brief Computes tidal corrections "f" and "u" for a group of tidal
/// constituents, given the mean longitudes of the sun's perihelion, lunar node,
/// lunar perigee, and the solar longitude.
///
/// The nodal corrections are a generalization of the standard nodal
/// corrections used in tidal analyses. Rather than accounting for sidelines
/// within a constituent (as the standard method does), this routine accounts
/// for sidelines within a tidal group. Only certain tidal groups can be so
/// handled; if this routine is called for a group not handled, it defaults to
/// the standard nodal corrections.
/// Only certain tidal groups can be so handled; if this routine is
/// called for a group not handled, it defaults to the standard nodal
/// corrections.
///
/// @param perihelion Mean longitude of the sun's perihelion, in degrees (for
/// times around J2000, this is about 282 degrees).
/// @param omega Mean longitude of the lunar node, in degrees.
/// @param perigee Mean longitude of the lunar perigee, in degrees.
/// @param hsolar Mean longitude of the sun, in degrees.
/// @param constituents A vector of constituents for which to compute
/// corrections.
/// @return NodalCorrections containing the computed corrections for
/// each constituent.
auto compute_nodal_corrections(double perihelion, double omega, double perigee,
                               double hsolar,
                               const std::vector<Constituent> &constituents)
    -> std::vector<NodalCorrections>;

}  // namespace perth
