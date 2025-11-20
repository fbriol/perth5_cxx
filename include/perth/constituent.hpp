#pragma once

#include <Eigen/Core>
#include <array>
#include <cstdint>
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
  kNode,
  kSa1,
  kSa,
  kSsa,
  kSta,
  kMSm,
  kMm,
  kMSf,
  kMf,
  kMStm,
  kMtm,
  kMSqm,
  kMqm,
  k2Q1,
  kSigma1,
  kQ1,
  kRho1,
  kO1,
  kTau1,
  kBeta1,
  kM13,
  kM1,
  kChi1,
  kPi1,
  kP1,
  kS1,
  kK1,
  kPsi1,
  kPhi1,
  kTheta1,
  kJ1,
  kSO1,
  kOO1,
  kUps1,
  kEps2,
  k2N2,
  k2MS2,
  kMu2,
  kN2,
  kN2P,
  kNu2,
  kGamma2,
  kAlpa2,
  kM2,
  kBeta2,
  kDelta2,
  kMKS2,
  kLambda2,
  k2MN2,
  kL2,
  kL2P,
  kT2,
  kS2,
  kR2,
  kK2,
  kMSN2,
  kEta2,
  k2SM2,
  k2MK3,
  kMO3,
  kM3,
  kMK3,
  kN4,
  kMN4,
  kM4,
  kSN4,
  kMS4,
  kMK4,
  kS4,
  kR4,
  kSK4,
  k2MN6,
  kM6,
  kMSN6,
  k2MS6,
  k2MK6,
  k2SM6,
  kMSK6,
  kS6,
  kM8,
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
class ConstituentSet {
 public:
  using value_type = T;
  using Key = std::array<Constituent, kNumConstituentItems>;
  using Item = std::array<T, kNumConstituentItems>;

  constexpr ConstituentSet() noexcept = default;

  constexpr ConstituentSet(Key &&keys, Item &&items) noexcept
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

  constexpr auto begin() const noexcept ->
      typename std::vector<Constituent>::const_iterator {
    return keys_vector_.begin();
  }

  constexpr auto end() const noexcept ->
      typename std::vector<Constituent>::const_iterator {
    return keys_vector_.end();
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

using ConstituentTable = ConstituentSet<TideComponent>;

auto assemble_constituent_table(
    const std::vector<Constituent> &constituents = {}) -> ConstituentTable;

auto constituent_to_name(Constituent constituent) -> std::string;

}  // namespace perth
