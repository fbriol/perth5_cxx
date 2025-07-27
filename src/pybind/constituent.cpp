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
      .value("_2MK3", perth::Constituent::k2MK3, "2MK3")
      .value("_2MK6", perth::Constituent::k2MK6, "2MK6")
      .value("_2MN2", perth::Constituent::k2MN2, "2MN2")
      .value("_2MN6", perth::Constituent::k2MN6, "2MN6")
      .value("_2MS2", perth::Constituent::k2MS2, "2MS2")
      .value("_2MS6", perth::Constituent::k2MS6, "2MS6")
      .value("_2N2", perth::Constituent::k2N2, "2N2")
      .value("_2Q1", perth::Constituent::k2Q1, "2Q1")
      .value("_2SM2", perth::Constituent::k2SM2, "2SM2")
      .value("_2SM6", perth::Constituent::k2SM6, "2SM6")
      .value("ALPHA2", perth::Constituent::kAlpa2, "Alpha2")
      .value("BETA2", perth::Constituent::kBeta2, "Beta2")
      .value("BETA1", perth::Constituent::kBeta1, "Beta1")
      .value("CHI1", perth::Constituent::kChi1, "Chi1")
      .value("DELTA2", perth::Constituent::kDelta2, "Delta2")
      .value("EPS2", perth::Constituent::kEps2, "Eps2")
      .value("ETA2", perth::Constituent::kEta2, "Eta2")
      .value("GAMMA2", perth::Constituent::kGamma2, "Gamma2")
      .value("J1", perth::Constituent::kJ1, "J1")
      .value("K1", perth::Constituent::kK1, "K1")
      .value("K2", perth::Constituent::kK2, "K2")
      .value("L2", perth::Constituent::kL2, "L2")
      .value("LAMBDA2", perth::Constituent::kLambda2, "Lambda2")
      .value("M1", perth::Constituent::kM1, "M1")
      .value("M13", perth::Constituent::kM13, "M13")
      .value("M2", perth::Constituent::kM2, "M2")
      .value("M3", perth::Constituent::kM3, "M3")
      .value("M4", perth::Constituent::kM4, "M4")
      .value("M6", perth::Constituent::kM6, "M6")
      .value("M8", perth::Constituent::kM8, "M8")
      .value("MF", perth::Constituent::kMf, "Mf")
      .value("MK3", perth::Constituent::kMK3, "MK3")
      .value("MK4", perth::Constituent::kMK4, "MK4")
      .value("MKS2", perth::Constituent::kMKS2, "MKS2")
      .value("MM", perth::Constituent::kMm, "Mm")
      .value("MN4", perth::Constituent::kMN4, "MN4")
      .value("MO3", perth::Constituent::kMO3, "MO3")
      .value("MQ", perth::Constituent::kMq, "Mq")
      .value("MS4", perth::Constituent::kMS4, "MS4")
      .value("MSF", perth::Constituent::kMSf, "MSf")
      .value("MSK6", perth::Constituent::kMSK6, "MSK6")
      .value("MSM", perth::Constituent::kMSm, "MSm")
      .value("MSN2", perth::Constituent::kMSN2, "MSN2")
      .value("MSN6", perth::Constituent::kMSN6, "MSN6")
      .value("MSQM", perth::Constituent::kMSqm, "MSqm")
      .value("MST", perth::Constituent::kMSt, "MSt")
      .value("MTM", perth::Constituent::kMtm, "Mtm")
      .value("MU2", perth::Constituent::kMu2, "Mu2")
      .value("N2", perth::Constituent::kN2, "N2")
      .value("N4", perth::Constituent::kN4, "N4")
      .value("NODE", perth::Constituent::kNode, "Node")
      .value("NU2", perth::Constituent::kNu2, "Nu2")
      .value("O1", perth::Constituent::kO1, "O1")
      .value("OO1", perth::Constituent::kOO1, "OO1")
      .value("P1", perth::Constituent::kP1, "P1")
      .value("PHI1", perth::Constituent::kPhi1, "Phi1")
      .value("PI1", perth::Constituent::kPi1, "Pi1")
      .value("PSI1", perth::Constituent::kPsi1, "Psi1")
      .value("Q1", perth::Constituent::kQ1, "Q1")
      .value("R2", perth::Constituent::kR2, "R2")
      .value("R4", perth::Constituent::kR4, "R4")
      .value("RHO1", perth::Constituent::kRho1, "Rho1")
      .value("S1", perth::Constituent::kS1, "S1")
      .value("S2", perth::Constituent::kS2, "S2")
      .value("S4", perth::Constituent::kS4, "S4")
      .value("S6", perth::Constituent::kS6, "S6")
      .value("SA", perth::Constituent::kSa, "Sa")
      .value("SA1", perth::Constituent::kSa1, "Sa1")
      .value("SIGMA1", perth::Constituent::kSigma1, "Sigma1")
      .value("SK4", perth::Constituent::kSK4, "SK4")
      .value("SN4", perth::Constituent::kSN4, "SN4")
      .value("SO1", perth::Constituent::kSO1, "SO1")
      .value("SSA", perth::Constituent::kSsa, "Ssa")
      .value("STA", perth::Constituent::kSta, "Sta")
      .value("T2", perth::Constituent::kT2, "T2")
      .value("TAU1", perth::Constituent::kTau1, "Tau1")
      .value("THETA1", perth::Constituent::kTheta1, "Theta1")
      .value("UPS1", perth::Constituent::kUps1, "Ups1");

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
