#include "constituent.hpp"

#include <nanobind/stl/optional.h>
#include <nanobind/stl/vector.h>

#include "perth/constituent.hpp"

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
      .value("NODE", perth::Constituent::kNode, "Node")
      .value("SA", perth::Constituent::kSa, "Sa")
      .value("SSA", perth::Constituent::kSsa, "Ssa")
      .value("STA", perth::Constituent::kSta, "Sta")
      .value("MSM", perth::Constituent::kMSm, "MSm")
      .value("MM", perth::Constituent::kMm, "Mm")
      .value("MSF", perth::Constituent::kMSf, "MSf")
      .value("MF", perth::Constituent::kMf, "Mf")
      .value("MST", perth::Constituent::kMSt, "MSt")
      .value("MT", perth::Constituent::kMt, "Mt")
      .value("MSQ", perth::Constituent::kMSq, "MSq")
      .value("MQ", perth::Constituent::kMq, "Mq")
      .value("_2Q1", perth::Constituent::k2Q1, "2Q1")
      .value("SIG1", perth::Constituent::kSig1, "Sig1")
      .value("Q1", perth::Constituent::kQ1, "Q1")
      .value("RHO1", perth::Constituent::kRho1, "Rho1")
      .value("O1", perth::Constituent::kO1, "O1")
      .value("TAU1", perth::Constituent::kTau1, "Tau1")
      .value("BET1", perth::Constituent::kBet1, "Bet1")
      .value("M1", perth::Constituent::kM1, "M1")
      .value("CHI1", perth::Constituent::kChi1, "Chi1")
      .value("I1", perth::Constituent::kPi1, "Pi1")
      .value("P1", perth::Constituent::kP1, "P1")
      .value("S1", perth::Constituent::kS1, "S1")
      .value("K1", perth::Constituent::kK1, "K1")
      .value("PSI1", perth::Constituent::kPsi1, "Psi1")
      .value("PHI1", perth::Constituent::kPhi1, "Phi1")
      .value("THE1", perth::Constituent::kThe1, "The1")
      .value("J1", perth::Constituent::kJ1, "J1")
      .value("SO1", perth::Constituent::kSO1, "SO1")
      .value("OO1", perth::Constituent::kOO1, "OO1")
      .value("UPS1", perth::Constituent::kUps1, "Ups1")
      .value("EPS2", perth::Constituent::kEps2, "Eps2")
      .value("_2N2", perth::Constituent::k2N2, "2N2")
      .value("MU2", perth::Constituent::kMu2, "Mu2")
      .value("N2", perth::Constituent::kN2, "N2")
      .value("NU2", perth::Constituent::kNu2, "Nu2")
      .value("GAM2", perth::Constituent::kGam2, "Gam2")
      .value("ALP2", perth::Constituent::kAlp2, "Alp2")
      .value("M2", perth::Constituent::kM2, "M2")
      .value("BET2", perth::Constituent::kBet2, "Bet2")
      .value("DEL2", perth::Constituent::kDel2, "Del2")
      .value("LAM2", perth::Constituent::kLam2, "Lam2")
      .value("L2", perth::Constituent::kL2, "L2")
      .value("T2", perth::Constituent::kT2, "T2")
      .value("S2", perth::Constituent::kS2, "S2")
      .value("R2", perth::Constituent::kR2, "R2")
      .value("K2", perth::Constituent::kK2, "K2")
      .value("ETA2", perth::Constituent::kEta2, "Eta2")
      .value("M4", perth::Constituent::kM4, "M4")
      .value("MS4", perth::Constituent::kMS4, "MS4");

  nb::class_<perth::TideTable>(m, "TideTable", "Table of tidal constituents.");

  m.def(
      "make_tide_table",
      [](const std::optional<std::vector<perth::Constituent>> &constituents) {
        if (constituents.has_value()) {
          return perth::make_tide_table(constituents.value());
        } else {
          return perth::make_tide_table();
        }
      },
      "Create a TideTable with the given constituents.",
      nb::arg("constituents") = std::nullopt, nb::rv_policy::automatic);
}
