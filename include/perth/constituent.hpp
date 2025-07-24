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
};

using TideTable = ConstituentArray<TideComponent>;

auto make_tide_table(const std::vector<Constituent> &constituents = {})
    -> TideTable;

}  // namespace perth
