#include <unordered_map>

#include "perth/constituent.hpp"

namespace perth {

struct Wave {
  Vector7b doodson_number;
  ConstituentType type;
};

static const std::array<std::pair<Constituent, Wave>,
                        Constituent::kNumConstituents>
    kConstituents{{
        {Constituent::k2MK3, {{3, -1, 0, 0, 0, 0, 3}, kShortPeriod}},
        {Constituent::k2MK6, {{6, 2, 0, 0, 0, 0, 0}, kShortPeriod}},
        {Constituent::k2MN2, {{2, 1, 0, -1, 0, 0, 0}, kShortPeriod}},
        {Constituent::k2MN6, {{6, -1, 0, 1, 0, 0, 0}, kShortPeriod}},
        {Constituent::k2MS2, {{2, -2, 2, 0, 0, 0, 0}, kShortPeriod}},
        {Constituent::k2MS6, {{6, 2, -2, 0, 0, 0, 0}, kShortPeriod}},
        {Constituent::k2N2, {{2, -2, 0, 2, 0, 0, 0}, kShortPeriod}},
        {Constituent::k2Q1, {{1, -3, 0, 2, 0, 0, 3}, kShortPeriod}},
        {Constituent::k2SM2, {{2, 4, -4, 0, 0, 0, 0}, kShortPeriod}},
        {Constituent::k2SM6, {{6, 4, -4, 0, 0, 0, 0}, kShortPeriod}},
        {Constituent::kAlp2, {{2, 0, -1, 0, 0, 1, 2}, kShortPeriod}},
        {Constituent::kBet2, {{2, 0, 1, 0, 0, -1, 0}, kShortPeriod}},
        {Constituent::kBeta1, {{1, 0, -2, 1, 0, 0, 1}, kShortPeriod}},
        {Constituent::kChi1, {{1, 0, 2, -1, 0, 0, 1}, kShortPeriod}},
        {Constituent::kDel2, {{2, 0, 2, 0, 0, 0, 0}, kShortPeriod}},
        {Constituent::kEps2, {{2, -3, 2, 1, 0, 0, 0}, kShortPeriod}},
        {Constituent::kEta2, {{2, 3, 0, -1, 0, 0, 0}, kShortPeriod}},
        {Constituent::kGam2, {{2, 0, -2, 2, 0, 0, 2}, kShortPeriod}},
        {Constituent::kJ1, {{1, 2, 0, -1, 0, 0, 1}, kShortPeriod}},
        {Constituent::kK1, {{1, 1, 0, 0, 0, 0, 1}, kShortPeriod}},
        {Constituent::kK2, {{2, 2, 0, 0, 0, 0, 0}, kShortPeriod}},
        {Constituent::kL2, {{2, 1, 0, -1, 0, 0, 2}, kShortPeriod}},
        {Constituent::kLambda2, {{2, 1, -2, 1, 0, 0, 2}, kShortPeriod}},
        {Constituent::kM1, {{1, 0, 0, 1, 0, 0, 1}, kShortPeriod}},
        {Constituent::kM13, {{1, 0, 0, 0, 0, 0, 2}, kShortPeriod}},
        {Constituent::kM2, {{2, 0, 0, 0, 0, 0, 0}, kShortPeriod}},
        {Constituent::kM3, {{3, 0, 0, 0, 0, 0, 2}, kShortPeriod}},
        {Constituent::kM4, {{4, 0, 0, 0, 0, 0, 0}, kShortPeriod}},
        {Constituent::kM6, {{6, 0, 0, 0, 0, 0, 0}, kShortPeriod}},
        {Constituent::kM8, {{8, 0, 0, 0, 0, 0, 0}, kShortPeriod}},
        {Constituent::kMK3, {{3, 1, 0, 0, 0, 0, 1}, kShortPeriod}},
        {Constituent::kMK4, {{4, 2, 0, 0, 0, 0, 0}, kShortPeriod}},
        {Constituent::kMKS2, {{2, 0, 2, 0, 0, 0, 0}, kShortPeriod}},
        {Constituent::kMN4, {{4, -1, 0, 1, 0, 0, 0}, kShortPeriod}},
        {Constituent::kMO3, {{3, -1, 0, 0, 0, 0, 3}, kShortPeriod}},
        {Constituent::kMS4, {{4, 2, -2, 0, 0, 0, 0}, kShortPeriod}},
        {Constituent::kMSK6, {{6, 4, -2, 0, 0, 0, 0}, kShortPeriod}},
        {Constituent::kMSN2, {{2, 3, -2, -1, 0, 0, 0}, kShortPeriod}},
        {Constituent::kMSN6, {{6, 1, -2, 1, 0, 0, 0}, kShortPeriod}},
        {Constituent::kMu2, {{2, -2, 2, 0, 0, 0, 0}, kShortPeriod}},
        {Constituent::kN2, {{2, -1, 0, 1, 0, 0, 0}, kShortPeriod}},
        {Constituent::kN4, {{4, -2, 0, 2, 0, 0, 0}, kShortPeriod}},
        {Constituent::kNu2, {{2, -1, 2, -1, 0, 0, 0}, kShortPeriod}},
        {Constituent::kO1, {{1, -1, 0, 0, 0, 0, 3}, kShortPeriod}},
        {Constituent::kOO1, {{1, 3, 0, 0, 0, 0, 1}, kShortPeriod}},
        {Constituent::kP1, {{1, 1, -2, 0, 0, 0, 3}, kShortPeriod}},
        {Constituent::kPhi1, {{1, 1, 2, 0, 0, 0, 1}, kShortPeriod}},
        {Constituent::kPi1, {{1, 1, -3, 0, 0, 1, 3}, kShortPeriod}},
        {Constituent::kPsi1, {{1, 1, 1, 0, 0, -1, 1}, kShortPeriod}},
        {Constituent::kQ1, {{1, -2, 0, 1, 0, 0, 3}, kShortPeriod}},
        {Constituent::kR2, {{2, 2, -1, 0, 0, -1, 2}, kShortPeriod}},
        {Constituent::kR4, {{4, 4, -3, 0, 0, 0, 0}, kShortPeriod}},
        {Constituent::kRho1, {{1, -2, 2, -1, 0, 0, 3}, kShortPeriod}},
        {Constituent::kS1, {{1, 1, -1, 0, 0, 0, 2}, kShortPeriod}},
        {Constituent::kS2, {{2, 2, -2, 0, 0, 0, 0}, kShortPeriod}},
        {Constituent::kS4, {{4, 4, -4, 0, 0, 0, 0}, kShortPeriod}},
        {Constituent::kS6, {{6, 6, -6, 0, 0, 0, 0}, kShortPeriod}},
        {Constituent::kSigma1, {{1, -3, 2, 0, 0, 0, 3}, kShortPeriod}},
        {Constituent::kSK4, {{4, 4, -2, 0, 0, 0, 0}, kShortPeriod}},
        {Constituent::kSN4, {{4, 1, -2, 1, 0, 0, 0}, kShortPeriod}},
        {Constituent::kSO1, {{1, 3, -2, 0, 0, 0, 1}, kShortPeriod}},
        {Constituent::kT2, {{2, 2, -3, 0, 0, 1, 0}, kShortPeriod}},
        {Constituent::kTau1, {{1, -1, 2, 0, 0, 0, 1}, kShortPeriod}},
        {Constituent::kTheta1, {{1, 2, -2, 1, 0, 0, 1}, kShortPeriod}},
        {Constituent::kUps1, {{1, 4, 0, -1, 0, 0, 1}, kShortPeriod}},
        {Constituent::kMf, {{0, 2, 0, 0, 0, 0, 0}, kLongPeriod}},
        {Constituent::kMm, {{0, 1, 0, -1, 0, 0, 0}, kLongPeriod}},
        {Constituent::kMq, {{0, 4, 0, -2, 0, 0, 0}, kLongPeriod}},
        {Constituent::kMSf, {{0, 2, -2, 0, 0, 0, 0}, kLongPeriod}},
        {Constituent::kMSm, {{0, 1, -2, 1, 0, 0, 0}, kLongPeriod}},
        {Constituent::kMSqm, {{0, 4, -2, 0, 0, 0, 0}, kLongPeriod}},
        {Constituent::kMSt, {{0, 3, -2, 1, 0, 0, 0}, kLongPeriod}},
        {Constituent::kMtm, {{0, 3, 0, -1, 0, 0, 0}, kLongPeriod}},
        {Constituent::kNode, {{0, 0, 0, 0, 1, 0, 2}, kLongPeriod}},
        {Constituent::kSa, {{0, 0, 1, 0, 0, -1, 0}, kLongPeriod}},
        {Constituent::kSa1, {{0, 0, 1, 0, 0, -1, 0}, kLongPeriod}},
        {Constituent::kSsa, {{0, 0, 2, 0, 0, 0, 0}, kLongPeriod}},
        {Constituent::kSta, {{0, 0, 3, 0, 0, 0, 0}, kLongPeriod}},
    }};

constexpr auto constituent_to_index(Constituent constituent) -> std::size_t {
  auto result = static_cast<std::size_t>(constituent);
  if (result >= Constituent::kNumConstituents) {
    throw std::out_of_range("Invalid constituent index");
  }
  return result;
}

auto make_tide_table(const std::vector<Constituent> &constituents)
    -> TideTable {
  TideTable::Item items;
  TideTable::Key keys;
  for (auto &&[key, item] : kConstituents) {
    auto index = constituent_to_index(key);
    auto inferred = std::find(constituents.begin(), constituents.end(), key);
    items[index] = {item.doodson_number,
                    {0, 0},
                    0,
                    item.type,
                    inferred == constituents.end()};
    keys[index] = key;
  }
  return TideTable(std::move(keys), std::move(items));
}

}  // namespace perth
