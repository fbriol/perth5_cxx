#include <unordered_map>

#include "perth/constituent.hpp"

namespace perth {

static const std::array<std::pair<Constituent, Data>,
                        Constituent::kNumConstituents>
    kConstituents{{
        {Constituent::kNode, {{0, 0, 0, 0, 1, 0}, 2}},
        {Constituent::kSa, {{0, 0, 1, 0, 0, -1}, 0}},
        {Constituent::kSsa, {{0, 0, 2, 0, 0, 0}, 0}},
        {Constituent::kSta, {{0, 0, 3, 0, 0, 0}, 0}},
        {Constituent::kMSm, {{0, 1, -2, 1, 0, 0}, 0}},
        {Constituent::kMm, {{0, 1, 0, -1, 0, 0}, 0}},
        {Constituent::kMSf, {{0, 2, -2, 0, 0, 0}, 0}},
        {Constituent::kMf, {{0, 2, 0, 0, 0, 0}, 0}},
        {Constituent::kMSt, {{0, 3, -2, 1, 0, 0}, 0}},
        {Constituent::kMt, {{0, 3, 0, -1, 0, 0}, 0}},
        {Constituent::kMSq, {{0, 4, -2, 0, 0, 0}, 0}},
        {Constituent::kMq, {{0, 4, 0, -2, 0, 0}, 0}},
        {Constituent::k2Q1, {{1, -3, 0, 2, 0, 0}, 3}},
        {Constituent::kSig1, {{1, -3, 2, 0, 0, 0}, 3}},
        {Constituent::kQ1, {{1, -2, 0, 1, 0, 0}, 3}},
        {Constituent::kRho1, {{1, -2, 2, -1, 0, 0}, 3}},
        {Constituent::kO1, {{1, -1, 0, 0, 0, 0}, 3}},
        {Constituent::kTau1, {{1, -1, 2, 0, 0, 0}, 1}},
        {Constituent::kBet1, {{1, 0, -2, 1, 0, 0}, 1}},
        {Constituent::kM1, {{1, 0, 0, 1, 0, 0}, 1}},
        {Constituent::kChi1, {{1, 0, 2, -1, 0, 0}, 1}},
        {Constituent::kPi1, {{1, 1, -3, 0, 0, 1}, 3}},
        {Constituent::kP1, {{1, 1, -2, 0, 0, 0}, 3}},
        {Constituent::kS1, {{1, 1, -1, 0, 0, 0}, 2}},
        {Constituent::kK1, {{1, 1, 0, 0, 0, 0}, 1}},
        {Constituent::kPsi1, {{1, 1, 1, 0, 0, -1}, 1}},
        {Constituent::kPhi1, {{1, 1, 2, 0, 0, 0}, 1}},
        {Constituent::kThe1, {{1, 2, -2, 1, 0, 0}, 1}},
        {Constituent::kJ1, {{1, 2, 0, -1, 0, 0}, 1}},
        {Constituent::kSO1, {{1, 3, -2, 0, 0, 0}, 1}},
        {Constituent::kOO1, {{1, 3, 0, 0, 0, 0}, 1}},
        {Constituent::kUps1, {{1, 4, 0, -1, 0, 0}, 1}},
        {Constituent::kEps2, {{2, -3, 2, 1, 0, 0}, 0}},
        {Constituent::k2N2, {{2, -2, 0, 2, 0, 0}, 0}},
        {Constituent::kMu2, {{2, -2, 2, 0, 0, 0}, 0}},
        {Constituent::kN2, {{2, -1, 0, 1, 0, 0}, 0}},
        {Constituent::kNu2, {{2, -1, 2, -1, 0, 0}, 0}},
        {Constituent::kGam2, {{2, 0, -2, 2, 0, 0}, 2}},
        {Constituent::kAlp2, {{2, 0, -1, 0, 0, 1}, 2}},
        {Constituent::kM2, {{2, 0, 0, 0, 0, 0}, 0}},
        {Constituent::kBet2, {{2, 0, 1, 0, 0, -1}, 0}},
        {Constituent::kDel2, {{2, 0, 2, 0, 0, 0}, 0}},
        {Constituent::kLam2, {{2, 1, -2, 1, 0, 0}, 2}},
        {Constituent::kL2, {{2, 1, 0, -1, 0, 0}, 2}},
        {Constituent::kT2, {{2, 2, -3, 0, 0, 1}, 0}},
        {Constituent::kS2, {{2, 2, -2, 0, 0, 0}, 0}},
        {Constituent::kR2, {{2, 2, -1, 0, 0, -1}, 2}},
        {Constituent::kK2, {{2, 2, 0, 0, 0, 0}, 0}},
        {Constituent::kEta2, {{2, 3, 0, -1, 0, 0}, 0}},
        {Constituent::kM4, {{4, 0, 0, 0, 0, 0}, 0}},
        {Constituent::kMS4, {{4, 2, -2, 0, 0, 0}, 0}},
    }};

constexpr auto constituent_to_index(Constituent constituent) -> std::size_t {
  auto result = static_cast<std::size_t>(constituent);
  if (result >= Constituent::kNumConstituents) {
    throw std::out_of_range("Invalid constituent index");
  }
  return result;
}

auto make_constituent_table() -> ConstituentTable {
  ConstituentTable::Item items;
  ConstituentTable::Key keys;
  for (auto &&[key, value] : kConstituents) {
    auto index = constituent_to_index(key);
    items[index] = std::move(value);
    keys[index] = key;
  }
  return ConstituentTable(std::move(keys), std::move(items));
}

auto make_tide_table(const std::vector<Constituent> &constituents)
    -> TideTable {
  TideTable::Item items;
  TideTable::Key keys;
  for (auto &&[key, value] : kConstituents) {
    auto index = constituent_to_index(key);
    auto inferred = std::find(constituents.begin(), constituents.end(), key);
    items[index] = {{0, 0}, inferred == constituents.end()};
    keys[index] = key;
  }
  return TideTable(std::move(keys), std::move(items));
}

}  // namespace perth
