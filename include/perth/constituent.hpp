#pragma once

#include <Eigen/Core>
#include <array>
#include <cstdint>
#include <vector>

#include "perth/eigen.hpp"

namespace perth {
enum Constituent : uint8_t {
  kNode = 0,
  kSa,
  kSsa,
  kSta,
  kMSm,
  kMm,
  kMSf,
  kMf,
  kMSt,
  kMt,
  kMSq,
  kMq,
  k2Q1,
  kSig1,
  kQ1,
  kRho1,
  kO1,
  kTau1,
  kBet1,
  kM1,
  kChi1,
  kPi1,
  kP1,
  kS1,
  kK1,
  kPsi1,
  kPhi1,
  kThe1,
  kJ1,
  kSO1,
  kOO1,
  kUps1,
  kEps2,
  k2N2,
  kMu2,
  kN2,
  kNu2,
  kGam2,
  kAlp2,
  kM2,
  kBet2,
  kDel2,
  kLam2,
  kL2,
  kT2,
  kS2,
  kR2,
  kK2,
  kEta2,
  kM4,
  kMS4,
  kNumConstituents,
};

constexpr std::size_t kNumConstituentItems =
    static_cast<std::size_t>(kNumConstituents);

template <typename T>
class ConstituentArray {
 public:
  using value_type = T;
  using Key = std::array<Constituent, kNumConstituentItems>;
  using Item = std::array<T, kNumConstituentItems>;

  constexpr ConstituentArray() noexcept = default;
  constexpr ConstituentArray(Key &&keys, Item &&items) noexcept
      : keys_(std::move(keys)), items_(std::move(items)) {}

  constexpr auto operator[](Constituent constituent) const noexcept
      -> const T & {
    return items_[static_cast<std::size_t>(constituent)];
  }

  constexpr auto operator[](Constituent constituent) noexcept -> T & {
    return items_[static_cast<std::size_t>(constituent)];
  }

  constexpr auto size() const noexcept -> std::size_t {
    return kNumConstituentItems;
  }

  constexpr auto keys() const noexcept -> const Key & { return keys_; }

  constexpr auto items() const noexcept -> const Item & { return items_; }

  constexpr auto items() noexcept -> Item & { return items_; }

  inline auto retrieve_keys_as_vector() const -> std::vector<Constituent> {
    return std::vector<Constituent>(keys_.begin(), keys_.end());
  }

 private:
  Key keys_{};
  Item items_{};
};

using Data = std::pair<Vector6b, int8_t>;
using ConstituentTable = ConstituentArray<Data>;
using TideTable = ConstituentArray<Complex>;

auto make_constituent_table() -> ConstituentTable;

auto make_tide_table() -> TideTable;

}  // namespace perth
