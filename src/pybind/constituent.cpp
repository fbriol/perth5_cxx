#include "constituent.hpp"

#include <nanobind/eigen/dense.h>
#include <nanobind/make_iterator.h>
#include <nanobind/stl/optional.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>

#include "perth/constituent.hpp"
#include "perth/tidal_frequency.hpp"

namespace nb = nanobind;

auto instantiate_constituent(nanobind::module_ &m) -> void {
  nb::enum_<perth::ConstituentType>(m, "ConstituentType",
                                    "Possible type of tidal wave.")
      .value("LONG_PERIOD", perth::ConstituentType::kLongPeriod,
             "Long period tidal waves")
      .value("SHORT_PERIOD", perth::ConstituentType::kShortPeriod,
             "Short period tidal waves");

  nb::enum_<perth::Constituent>(m, "Constituent",
                                "Enum representing tidal constituents.")
      .value("_2MK3", perth::Constituent::k2MK3,
             perth::constituent_to_name(perth::Constituent::k2MK3).c_str())
      .value("_2MK6", perth::Constituent::k2MK6,
             perth::constituent_to_name(perth::Constituent::k2MK6).c_str())
      .value("_2MN2", perth::Constituent::k2MN2,
             perth::constituent_to_name(perth::Constituent::k2MN2).c_str())
      .value("_2MN6", perth::Constituent::k2MN6,
             perth::constituent_to_name(perth::Constituent::k2MN6).c_str())
      .value("_2MS2", perth::Constituent::k2MS2,
             perth::constituent_to_name(perth::Constituent::k2MS2).c_str())
      .value("_2MS6", perth::Constituent::k2MS6,
             perth::constituent_to_name(perth::Constituent::k2MS6).c_str())
      .value("_2N2", perth::Constituent::k2N2,
             perth::constituent_to_name(perth::Constituent::k2N2).c_str())
      .value("_2Q1", perth::Constituent::k2Q1,
             perth::constituent_to_name(perth::Constituent::k2Q1).c_str())
      .value("_2SM2", perth::Constituent::k2SM2,
             perth::constituent_to_name(perth::Constituent::k2SM2).c_str())
      .value("_2SM6", perth::Constituent::k2SM6,
             perth::constituent_to_name(perth::Constituent::k2SM6).c_str())
      .value("ALPHA2", perth::Constituent::kAlpa2,
             perth::constituent_to_name(perth::Constituent::kAlpa2).c_str())
      .value("BETA2", perth::Constituent::kBeta2,
             perth::constituent_to_name(perth::Constituent::kBeta2).c_str())
      .value("BETA1", perth::Constituent::kBeta1,
             perth::constituent_to_name(perth::Constituent::kBeta1).c_str())
      .value("CHI1", perth::Constituent::kChi1,
             perth::constituent_to_name(perth::Constituent::kChi1).c_str())
      .value("DELTA2", perth::Constituent::kDelta2,
             perth::constituent_to_name(perth::Constituent::kDelta2).c_str())
      .value("EPS2", perth::Constituent::kEps2,
             perth::constituent_to_name(perth::Constituent::kEps2).c_str())
      .value("ETA2", perth::Constituent::kEta2,
             perth::constituent_to_name(perth::Constituent::kEta2).c_str())
      .value("GAMMA2", perth::Constituent::kGamma2,
             perth::constituent_to_name(perth::Constituent::kGamma2).c_str())
      .value("J1", perth::Constituent::kJ1,
             perth::constituent_to_name(perth::Constituent::kJ1).c_str())
      .value("K1", perth::Constituent::kK1,
             perth::constituent_to_name(perth::Constituent::kK1).c_str())
      .value("K2", perth::Constituent::kK2,
             perth::constituent_to_name(perth::Constituent::kK2).c_str())
      .value("L2", perth::Constituent::kL2,
             perth::constituent_to_name(perth::Constituent::kL2).c_str())
      .value("LAMBDA2", perth::Constituent::kLambda2,
             perth::constituent_to_name(perth::Constituent::kLambda2).c_str())
      .value("M1", perth::Constituent::kM1,
             perth::constituent_to_name(perth::Constituent::kM1).c_str())
      .value("M13", perth::Constituent::kM13,
             perth::constituent_to_name(perth::Constituent::kM13).c_str())
      .value("M2", perth::Constituent::kM2,
             perth::constituent_to_name(perth::Constituent::kM2).c_str())
      .value("M3", perth::Constituent::kM3,
             perth::constituent_to_name(perth::Constituent::kM3).c_str())
      .value("M4", perth::Constituent::kM4,
             perth::constituent_to_name(perth::Constituent::kM4).c_str())
      .value("M6", perth::Constituent::kM6,
             perth::constituent_to_name(perth::Constituent::kM6).c_str())
      .value("M8", perth::Constituent::kM8,
             perth::constituent_to_name(perth::Constituent::kM8).c_str())
      .value("MF", perth::Constituent::kMf,
             perth::constituent_to_name(perth::Constituent::kMf).c_str())
      .value("MK3", perth::Constituent::kMK3,
             perth::constituent_to_name(perth::Constituent::kMK3).c_str())
      .value("MK4", perth::Constituent::kMK4,
             perth::constituent_to_name(perth::Constituent::kMK4).c_str())
      .value("MKS2", perth::Constituent::kMKS2,
             perth::constituent_to_name(perth::Constituent::kMKS2).c_str())
      .value("MM", perth::Constituent::kMm,
             perth::constituent_to_name(perth::Constituent::kMm).c_str())
      .value("MN4", perth::Constituent::kMN4,
             perth::constituent_to_name(perth::Constituent::kMN4).c_str())
      .value("MO3", perth::Constituent::kMO3,
             perth::constituent_to_name(perth::Constituent::kMO3).c_str())
      .value("MQM", perth::Constituent::kMqm,
             perth::constituent_to_name(perth::Constituent::kMqm).c_str())
      .value("MS4", perth::Constituent::kMS4,
             perth::constituent_to_name(perth::Constituent::kMS4).c_str())
      .value("MSF", perth::Constituent::kMSf,
             perth::constituent_to_name(perth::Constituent::kMSf).c_str())
      .value("MSK6", perth::Constituent::kMSK6,
             perth::constituent_to_name(perth::Constituent::kMSK6).c_str())
      .value("MSM", perth::Constituent::kMSm,
             perth::constituent_to_name(perth::Constituent::kMSm).c_str())
      .value("MSN2", perth::Constituent::kMSN2,
             perth::constituent_to_name(perth::Constituent::kMSN2).c_str())
      .value("MSN6", perth::Constituent::kMSN6,
             perth::constituent_to_name(perth::Constituent::kMSN6).c_str())
      .value("MSQM", perth::Constituent::kMSqm,
             perth::constituent_to_name(perth::Constituent::kMSqm).c_str())
      .value("MSTM", perth::Constituent::kMStm,
             perth::constituent_to_name(perth::Constituent::kMStm).c_str())
      .value("MTM", perth::Constituent::kMtm,
             perth::constituent_to_name(perth::Constituent::kMtm).c_str())
      .value("MU2", perth::Constituent::kMu2,
             perth::constituent_to_name(perth::Constituent::kMu2).c_str())
      .value("N2", perth::Constituent::kN2,
             perth::constituent_to_name(perth::Constituent::kN2).c_str())
      .value("N4", perth::Constituent::kN4,
             perth::constituent_to_name(perth::Constituent::kN4).c_str())
      .value("NODE", perth::Constituent::kNode,
             perth::constituent_to_name(perth::Constituent::kNode).c_str())
      .value("NU2", perth::Constituent::kNu2,
             perth::constituent_to_name(perth::Constituent::kNu2).c_str())
      .value("O1", perth::Constituent::kO1,
             perth::constituent_to_name(perth::Constituent::kO1).c_str())
      .value("OO1", perth::Constituent::kOO1,
             perth::constituent_to_name(perth::Constituent::kOO1).c_str())
      .value("P1", perth::Constituent::kP1,
             perth::constituent_to_name(perth::Constituent::kP1).c_str())
      .value("PHI1", perth::Constituent::kPhi1,
             perth::constituent_to_name(perth::Constituent::kPhi1).c_str())
      .value("PI1", perth::Constituent::kPi1,
             perth::constituent_to_name(perth::Constituent::kPi1).c_str())
      .value("PSI1", perth::Constituent::kPsi1,
             perth::constituent_to_name(perth::Constituent::kPsi1).c_str())
      .value("Q1", perth::Constituent::kQ1,
             perth::constituent_to_name(perth::Constituent::kQ1).c_str())
      .value("R2", perth::Constituent::kR2,
             perth::constituent_to_name(perth::Constituent::kR2).c_str())
      .value("R4", perth::Constituent::kR4,
             perth::constituent_to_name(perth::Constituent::kR4).c_str())
      .value("RHO1", perth::Constituent::kRho1,
             perth::constituent_to_name(perth::Constituent::kRho1).c_str())
      .value("S1", perth::Constituent::kS1,
             perth::constituent_to_name(perth::Constituent::kS1).c_str())
      .value("S2", perth::Constituent::kS2,
             perth::constituent_to_name(perth::Constituent::kS2).c_str())
      .value("S4", perth::Constituent::kS4,
             perth::constituent_to_name(perth::Constituent::kS4).c_str())
      .value("S6", perth::Constituent::kS6,
             perth::constituent_to_name(perth::Constituent::kS6).c_str())
      .value("SA", perth::Constituent::kSa,
             perth::constituent_to_name(perth::Constituent::kSa).c_str())
      .value("SA1", perth::Constituent::kSa1,
             perth::constituent_to_name(perth::Constituent::kSa1).c_str())
      .value("SIGMA1", perth::Constituent::kSigma1,
             perth::constituent_to_name(perth::Constituent::kSigma1).c_str())
      .value("SK4", perth::Constituent::kSK4,
             perth::constituent_to_name(perth::Constituent::kSK4).c_str())
      .value("SN4", perth::Constituent::kSN4,
             perth::constituent_to_name(perth::Constituent::kSN4).c_str())
      .value("SO1", perth::Constituent::kSO1,
             perth::constituent_to_name(perth::Constituent::kSO1).c_str())
      .value("SSA", perth::Constituent::kSsa,
             perth::constituent_to_name(perth::Constituent::kSsa).c_str())
      .value("STA", perth::Constituent::kSta,
             perth::constituent_to_name(perth::Constituent::kSta).c_str())
      .value("T2", perth::Constituent::kT2,
             perth::constituent_to_name(perth::Constituent::kT2).c_str())
      .value("TAU1", perth::Constituent::kTau1,
             perth::constituent_to_name(perth::Constituent::kTau1).c_str())
      .value("THETA1", perth::Constituent::kTheta1,
             perth::constituent_to_name(perth::Constituent::kTheta1).c_str())
      .value("UPS1", perth::Constituent::kUps1,
             perth::constituent_to_name(perth::Constituent::kUps1).c_str());

  nb::class_<perth::TideComponent>(m, "TideComponent",
                                   "Represents a tidal component.")
      .def_prop_ro(
          "doodson_number",
          [](const perth::TideComponent &self) -> Eigen::Vector<int8_t, 7> {
            return self.doodson_number;
          },
          "Doodson number of the constituent.")
      .def_prop_ro(
          "tide",
          [](const perth::TideComponent &self) -> std::complex<double> {
            return self.tide;
          },
          "Tide of the constituent.")
      .def_prop_ro(
          "tidal_argument",
          [](const perth::TideComponent &self) -> double {
            return self.tidal_argument;
          },
          "Doodson argument of the constituent.")
      .def_prop_ro(
          "type",
          [](const perth::TideComponent &self) -> perth::ConstituentType {
            return self.type;
          },
          "Type of tidal wave.")
      .def_prop_ro(
          "is_inferred",
          [](const perth::TideComponent &self) -> bool {
            return self.is_inferred;
          },
          "Whether the tide was inferred from the constituents.");

  nb::class_<perth::ConstituentTable>(m, "ConstituentTable",
                                      "Table of tidal constituents.")
      .def(
          "__len__",
          [](const perth::ConstituentTable &self) -> size_t {
            return self.size();
          },
          "Returns the number of constituents in the table.")
      .def(
          "__getitem__",
          [](const perth::ConstituentTable &self,
             perth::Constituent constituent) -> const perth::TideComponent & {
            return self[constituent];
          },
          "Get a constituent by its enum value.")
      .def(
          "__iter__",
          [](const perth::ConstituentTable &self) {
            return nb::make_iterator(nb::type<perth::ConstituentTable>(),
                                     "key_iterator", self.begin(), self.end());
          },
          nb::keep_alive<0, 1>());

  m.def("tidal_frequency", &perth::tidal_frequency,
        "Calculate the tidal frequency in degrees per hour from a Doodson "
        "number.",
        nb::arg("doodson_number"),
        "Returns the tidal frequency in degrees per hour.");

  m.def(
      "assemble_constituent_table",
      [](const std::optional<std::vector<perth::Constituent>> &constituents) {
        if (constituents.has_value()) {
          return perth::assemble_constituent_table(constituents.value());
        } else {
          return perth::assemble_constituent_table();
        }
      },
      "Create a TideTable with the given constituents.",
      nb::arg("constituents") = std::nullopt, nb::rv_policy::automatic);

  m.def("constituent_to_name", &perth::constituent_to_name,
        "Convert a Constituent enum value to its string name.",
        nb::arg("constituent"),
        "Returns the name of the constituent as a string.");
}
