#include <unordered_map>

#include "perth/constituent.hpp"

namespace perth {

struct Wave {
  Data data;
  ConstituentType type;
};

static const std::array<std::pair<Constituent, Wave>,
                        Constituent::kNumConstituents>
    kConstituents{{
        {Constituent::kNode, {{{0, 0, 0, 0, 1, 0}, 2}, kLongPeriod}},
        {Constituent::kSa, {{{0, 0, 1, 0, 0, -1}, 0}, kLongPeriod}},
        {Constituent::kSsa, {{{0, 0, 2, 0, 0, 0}, 0}, kLongPeriod}},
        {Constituent::kSta, {{{0, 0, 3, 0, 0, 0}, 0}, kLongPeriod}},
        {Constituent::kMSm, {{{0, 1, -2, 1, 0, 0}, 0}, kLongPeriod}},
        {Constituent::kMm, {{{0, 1, 0, -1, 0, 0}, 0}, kLongPeriod}},
        {Constituent::kMSf, {{{0, 2, -2, 0, 0, 0}, 0}, kLongPeriod}},
        {Constituent::kMf, {{{0, 2, 0, 0, 0, 0}, 0}, kLongPeriod}},
        {Constituent::kMSt, {{{0, 3, -2, 1, 0, 0}, 0}, kLongPeriod}},
        {Constituent::kMt, {{{0, 3, 0, -1, 0, 0}, 0}, kLongPeriod}},
        {Constituent::kMSq, {{{0, 4, -2, 0, 0, 0}, 0}, kLongPeriod}},
        {Constituent::kMq, {{{0, 4, 0, -2, 0, 0}, 0}, kLongPeriod}},
        {Constituent::k2Q1, {{{1, -3, 0, 2, 0, 0}, 3}, kShortPeriod}},
        {Constituent::kSig1, {{{1, -3, 2, 0, 0, 0}, 3}, kShortPeriod}},
        {Constituent::kQ1, {{{1, -2, 0, 1, 0, 0}, 3}, kShortPeriod}},
        {Constituent::kRho1, {{{1, -2, 2, -1, 0, 0}, 3}, kShortPeriod}},
        {Constituent::kO1, {{{1, -1, 0, 0, 0, 0}, 3}, kShortPeriod}},
        {Constituent::kTau1, {{{1, -1, 2, 0, 0, 0}, 1}, kShortPeriod}},
        {Constituent::kBet1, {{{1, 0, -2, 1, 0, 0}, 1}, kShortPeriod}},
        {Constituent::kM1, {{{1, 0, 0, 1, 0, 0}, 1}, kShortPeriod}},
        {Constituent::kChi1, {{{1, 0, 2, -1, 0, 0}, 1}, kShortPeriod}},
        {Constituent::kPi1, {{{1, 1, -3, 0, 0, 1}, 3}, kShortPeriod}},
        {Constituent::kP1, {{{1, 1, -2, 0, 0, 0}, 3}, kShortPeriod}},
        {Constituent::kS1, {{{1, 1, -1, 0, 0, 0}, 2}, kShortPeriod}},
        {Constituent::kK1, {{{1, 1, 0, 0, 0, 0}, 1}, kShortPeriod}},
        {Constituent::kPsi1, {{{1, 1, 1, 0, 0, -1}, 1}, kShortPeriod}},
        {Constituent::kPhi1, {{{1, 1, 2, 0, 0, 0}, 1}, kShortPeriod}},
        {Constituent::kThe1, {{{1, 2, -2, 1, 0, 0}, 1}, kShortPeriod}},
        {Constituent::kJ1, {{{1, 2, 0, -1, 0, 0}, 1}, kShortPeriod}},
        {Constituent::kSO1, {{{1, 3, -2, 0, 0, 0}, 1}, kShortPeriod}},
        {Constituent::kOO1, {{{1, 3, 0, 0, 0, 0}, 1}, kShortPeriod}},
        {Constituent::kUps1, {{{1, 4, 0, -1, 0, 0}, 1}, kShortPeriod}},
        {Constituent::kEps2, {{{2, -3, 2, 1, 0, 0}, 0}, kShortPeriod}},
        {Constituent::k2N2, {{{2, -2, 0, 2, 0, 0}, 0}, kShortPeriod}},
        {Constituent::kMu2, {{{2, -2, 2, 0, 0, 0}, 0}, kShortPeriod}},
        {Constituent::kN2, {{{2, -1, 0, 1, 0, 0}, 0}, kShortPeriod}},
        {Constituent::kNu2, {{{2, -1, 2, -1, 0, 0}, 0}, kShortPeriod}},
        {Constituent::kGam2, {{{2, 0, -2, 2, 0, 0}, 2}, kShortPeriod}},
        {Constituent::kAlp2, {{{2, 0, -1, 0, 0, 1}, 2}, kShortPeriod}},
        {Constituent::kM2, {{{2, 0, 0, 0, 0, 0}, 0}, kShortPeriod}},
        {Constituent::kBet2, {{{2, 0, 1, 0, 0, -1}, 0}, kShortPeriod}},
        {Constituent::kDel2, {{{2, 0, 2, 0, 0, 0}, 0}, kShortPeriod}},
        {Constituent::kLam2, {{{2, 1, -2, 1, 0, 0}, 2}, kShortPeriod}},
        {Constituent::kL2, {{{2, 1, 0, -1, 0, 0}, 2}, kShortPeriod}},
        {Constituent::kT2, {{{2, 2, -3, 0, 0, 1}, 0}, kShortPeriod}},
        {Constituent::kS2, {{{2, 2, -2, 0, 0, 0}, 0}, kShortPeriod}},
        {Constituent::kR2, {{{2, 2, -1, 0, 0, -1}, 2}, kShortPeriod}},
        {Constituent::kK2, {{{2, 2, 0, 0, 0, 0}, 0}, kShortPeriod}},
        {Constituent::kEta2, {{{2, 3, 0, -1, 0, 0}, 0}, kShortPeriod}},
        {Constituent::kM4, {{{4, 0, 0, 0, 0, 0}, 0}, kShortPeriod}},
        {Constituent::kMS4, {{{4, 2, -2, 0, 0, 0}, 0}, kShortPeriod}},
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
  for (auto &[key, item] : kConstituents) {
    auto index = constituent_to_index(key);
    items[index] = item.data;
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
    items[index] = {{0, 0}, value.type, inferred == constituents.end()};
    keys[index] = key;
  }
  return TideTable(std::move(keys), std::move(items));
}

}  // namespace perth
