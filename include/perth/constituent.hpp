#pragma once

#include <Eigen/Core>
#include <array>
#include <cstdint>
#include <optional>
#include <vector>

#include "perth/eigen.hpp"

namespace perth {

/// @brief Possible type of tidal wave.
enum ConstituentType : uint8_t {
  kLongPeriod = 0,  //!< Long period tidal waves
  kShortPeriod      //!< Short period tidal waves
};

/// @brief Enum class representing tidal constituents.
enum Constituent : uint8_t {
  k2MK3 = 0,
  k2MK6,
  k2MN2,
  k2MN6,
  k2MS2,
  k2MS6,
  k2N2,
  k2Q1,
  k2SM2,
  k2SM6,
  kAlp2,
  kBet2,
  kBeta1,
  kChi1,
  kDel2,
  kEps2,
  kEta2,
  kGam2,
  kJ1,
  kK1,
  kK2,
  kL2,
  kLambda2,
  kM1,
  kM13,
  kM2,
  kM3,
  kM4,
  kM6,
  kM8,
  kMf,
  kMK3,
  kMK4,
  kMKS2,
  kMm,
  kMN4,
  kMO3,
  kMq,
  kMS4,
  kMSf,
  kMSK6,
  kMSm,
  kMSN2,
  kMSN6,
  kMSqm,
  kMSt,
  kMtm,
  kMu2,
  kN2,
  kN4,
  kNode,
  kNu2,
  kO1,
  kOO1,
  kP1,
  kPhi1,
  kPi1,
  kPsi1,
  kQ1,
  kR2,
  kR4,
  kRho1,
  kS1,
  kS2,
  kS4,
  kS6,
  kSa,
  kSa1,
  kSigma1,
  kSK4,
  kSN4,
  kSO1,
  kSsa,
  kSta,
  kT2,
  kTau1,
  kTheta1,
  kUps1,
  kNumConstituents,
};

/// @brief Data structure that holds the tide of a constituent.
struct TideComponent {
  Vector7b doodson_number;  //!< Doodson number of the constituent
  Complex tide;             //!< Tide of the constituent

  double tidal_argument;     //!< Doodson argument
  ConstituentType type;      //!< Type of tidal wave
  bool is_inferred = false;  //!< Whether the tide was inferred from the
                             //!< constituents
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
      : keys_(std::move(keys)),
        items_(std::move(items)),
        keys_vector_(keys_.begin(), keys_.end()) {}

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

  constexpr auto keys_vector() const -> const std::vector<Constituent> & {
    return keys_vector_;
  }

  constexpr auto at(const size_t index) const
      -> const std::pair<Constituent, T> {
    if (index >= static_cast<size_t>(kNumConstituentItems)) {
      throw std::out_of_range("Index out of range");
    }
    return {keys_[index], items_[index]};
  }

 private:
  Key keys_{};
  Item items_{};
  std::vector<Constituent> keys_vector_;
};

using TideTable = ConstituentArray<TideComponent>;

auto make_tide_table(const std::vector<Constituent> &constituents = {})
    -> TideTable;

}  // namespace perth
