#pragma once

#include <functional>
#include <unordered_map>

#include "perth/constituent.hpp"
#include "perth/eigen.hpp"

namespace perth {

enum class InterpolationType {
  kLinearAdmittance,   ///< Piecewise linear interpolation of admittances.
  kFourierAdmittance,  ///< Munk-Cartwright Fourier series interpolation.
};

enum class InputType {
  kHormonic,  ///<  harmonic constants are inphase/quadrature components.
  kAmplitude  ///< constants are amplitudes, phase lags (deg).
};

enum class CalculationType {
  kMinor,            ///< Minor constituents (diurnal and semidiurnal).
  kMinorLongPeriod,  ///< Long-period constituents.
};

class Inference {
 public:
  Inference(InterpolationType interpolation_type, InputType input_type,
            CalculationType calculation_type,
            const ConstituentTable& constituents);

  auto operator()(TideTable& hc) const -> void;

 private:
  /// @brief Type for interpolation functions.
  /// The function takes in the frequencies and amplitudes of three components
  /// and returns a complex number representing the interpolated value.
  using Interpolator = std::function<Complex(double, double, double, double,
                                             double, double, double)>;

  static std::unordered_map<Constituent, double>
      kInferredDiurnalConstituents_;  ///< Array of inferred diurnal
                                      ///< constituents with their frequencies.

  static std::unordered_map<Constituent, double>
      kInferredSemidiurnalConstituents_;  ///< Array of inferred semidiurnal
                                          ///< constituents with their
                                          ///< frequencies.

  static std::unordered_map<Constituent, double>
      kInferredLongPeriodConstituents_;  ///< Array of inferred long-period
                                         ///< constituents with their
                                         ///< frequencies.

  std::unordered_map<Constituent, std::pair<double, double>>
      inferred_diurnal_;  ///< Inferred diurnal constituents with their
                          ///< frequencies.
  std::unordered_map<Constituent, std::pair<double, double>>
      inferred_semidiurnal_;  ///< Inferred semidiurnal constituents with their
                              ///< frequencies.
  std::unordered_map<Constituent, std::pair<double, double>>
      inferred_long_period_;  ///< Inferred long-period constituents with their
                              ///< frequencies.

  std::vector<Constituent> diurnal_keys_;  ///< Keys for diurnal constituents
                                           ///< ordered by frequency.
  std::vector<Constituent> semidiurnal_keys_;  ///< Keys for semidiurnal
                                               ///< constituents ordered by
                                               ///< frequency.
  std::vector<Constituent> long_period_keys_;  ///< Keys for long-period
                                               ///< constituents ordered by
                                               ///< frequency.

  double x1_;             ///< Frequency for the first component.
  double x2_;             ///< Frequency for the second component.
  double x3_;             ///< Frequency for the third component.
  double x4_;             ///< Frequency for the first component (semidurnal).
  double x5_;             ///< Frequency for the second component (semidurnal).
  double x6_;             ///< Frequency for the third component (semidurnal).
  double x7_;             ///< Frequency for the first component (long-period).
  double x8_;             ///< Frequency for the second component (long-period).
  double x9_;             ///< Frequency for the third component (long-period).
  double amp1_;           ///< Amplitude for the first component.
  double amp2_;           ///< Amplitude for the second component.
  double amp3_;           ///< Amplitude for the third component.
  double amp4_;           ///< Amplitude for the first component (semidurnal).
  double amp5_;           ///< Amplitude for the second component (semidurnal).
  double amp6_;           ///< Amplitude for the third component (semidurnal).
  double amp7_;           ///< Amplitude for the first component (long-period).
  double amp8_;           ///< Amplitude for the second component (long-period).
  double amp9_;           ///< Amplitude for the third component (long-period).
  InputType input_type_;  ///< Type of input data (harmonic or amplitude).
  CalculationType calculation_type_;  ///< Type of calculation to perform.
  std::function<Complex(double, const Complex&, double, const Complex&, double,
                        const Complex&,
                        double)>
      interpolation_1_;  ///< Interpolation function to use.
  std::function<Complex(double, const Complex&, double, const Complex&, double,
                        const Complex&,
                        double)>
      interpolation_2_;  ///< Interpolation function to use for semidurnal
};

}  // namespace perth
