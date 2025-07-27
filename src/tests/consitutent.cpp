#include <gtest/gtest.h>

#include <complex>

#include "perth/constituent.hpp"

namespace perth {

// Test fixture for ConstituentArray tests
template <typename T>
class ConstituentArrayTest : public ::testing::Test {
 protected:
  using TestArray = ConstituentSet<T>;
};

// Test types
using TestTypes = ::testing::Types<int, double, std::complex<double>>;
TYPED_TEST_SUITE(ConstituentArrayTest, TestTypes);

// Test default constructor
TYPED_TEST(ConstituentArrayTest, DefaultConstructor) {
  typename TestFixture::TestArray array;
  EXPECT_EQ(array.size(), kNumConstituentItems);
  EXPECT_EQ(array.size(),
            static_cast<std::size_t>(Constituent::kNumConstituents));
}

// Test constructor with keys and items
TYPED_TEST(ConstituentArrayTest, ConstructorWithKeysAndItems) {
  typename TestFixture::TestArray::Key keys;
  typename TestFixture::TestArray::Item items;

  // Fill with test data
  for (std::size_t i = 0; i < kNumConstituentItems; ++i) {
    keys[i] = static_cast<Constituent>(i);
    if constexpr (std::is_same_v<TypeParam, std::complex<double>>) {
      items[i] = {static_cast<double>(i), static_cast<double>(i + 1)};
    } else {
      items[i] = static_cast<TypeParam>(i);
    }
  }

  typename TestFixture::TestArray array(std::move(keys), std::move(items));

  EXPECT_EQ(array.size(), kNumConstituentItems);

  // Verify data integrity
  for (std::size_t i = 0; i < kNumConstituentItems; ++i) {
    Constituent constituent = static_cast<Constituent>(i);
    if constexpr (std::is_same_v<TypeParam, std::complex<double>>) {
      EXPECT_EQ(array[constituent].real(), static_cast<double>(i));
      EXPECT_EQ(array[constituent].imag(), static_cast<double>(i + 1));
    } else {
      EXPECT_EQ(array[constituent], static_cast<TypeParam>(i));
    }
  }
}

// Test const and non-const operator[]
TYPED_TEST(ConstituentArrayTest, IndexOperator) {
  typename TestFixture::TestArray array;

  // Test non-const operator[]
  if constexpr (std::is_same_v<TypeParam, std::complex<double>>) {
    array[Constituent::kNode] = {1.5, 2.5};
    array[Constituent::kM2] = {3.7, 4.2};
  } else {
    array[Constituent::kNode] = static_cast<TypeParam>(42);
    array[Constituent::kM2] = static_cast<TypeParam>(100);
  }

  // Test const operator[]
  const auto& const_array = array;
  if constexpr (std::is_same_v<TypeParam, std::complex<double>>) {
    EXPECT_EQ(const_array[Constituent::kNode].real(), 1.5);
    EXPECT_EQ(const_array[Constituent::kNode].imag(), 2.5);
    EXPECT_EQ(const_array[Constituent::kM2].real(), 3.7);
    EXPECT_EQ(const_array[Constituent::kM2].imag(), 4.2);
  } else {
    EXPECT_EQ(const_array[Constituent::kNode], static_cast<TypeParam>(42));
    EXPECT_EQ(const_array[Constituent::kM2], static_cast<TypeParam>(100));
  }
}

// Test keys() method
TYPED_TEST(ConstituentArrayTest, Keys) {
  typename TestFixture::TestArray::Key test_keys;
  typename TestFixture::TestArray::Item test_items;

  // Fill with specific test data
  test_keys[0] = Constituent::kNode;
  test_keys[1] = Constituent::kSa;
  test_keys[2] = Constituent::kM2;

  typename TestFixture::TestArray array(std::move(test_keys),
                                        std::move(test_items));

  const auto& keys = array.keys();
  EXPECT_EQ(keys[0], Constituent::kNode);
  EXPECT_EQ(keys[1], Constituent::kSa);
  EXPECT_EQ(keys[2], Constituent::kM2);
}

// Test items() methods
TYPED_TEST(ConstituentArrayTest, Items) {
  typename TestFixture::TestArray array;

  // Test non-const items()
  auto& items = array.items();
  if constexpr (std::is_same_v<TypeParam, std::complex<double>>) {
    items[0] = {1.0, 2.0};
  } else {
    items[0] = static_cast<TypeParam>(123);
  }

  // Test const items()
  const auto& const_array = array;
  const auto& const_items = const_array.items();
  if constexpr (std::is_same_v<TypeParam, std::complex<double>>) {
    EXPECT_EQ(const_items[0].real(), 1.0);
    EXPECT_EQ(const_items[0].imag(), 2.0);
  } else {
    EXPECT_EQ(const_items[0], static_cast<TypeParam>(123));
  }
}

// Test keys_vector method
TYPED_TEST(ConstituentArrayTest, RetrieveKeysAsVector) {
  typename TestFixture::TestArray::Key test_keys;
  typename TestFixture::TestArray::Item test_items;

  // Fill keys with specific constituents
  for (std::size_t i = 0; i < kNumConstituentItems; ++i) {
    test_keys[i] = static_cast<Constituent>(i);
  }

  typename TestFixture::TestArray array(std::move(test_keys),
                                        std::move(test_items));

  auto keys_vector = array.keys_vector();
  EXPECT_EQ(keys_vector.size(), kNumConstituentItems);

  for (std::size_t i = 0; i < kNumConstituentItems; ++i) {
    EXPECT_EQ(keys_vector[i], static_cast<Constituent>(i));
  }
}

// Tests for make_tide_table function
class TideTableTest : public ::testing::Test {
 private:
  static std::vector<Constituent> enabled_;

 protected:
  ConstituentTable table = assemble_constituent_table(TideTableTest::enabled_);

  auto is_enabled(Constituent constituent) const -> bool {
    return std::find(enabled_.begin(), enabled_.end(), constituent) !=
           enabled_.end();
  }
};

std::vector<Constituent> TideTableTest::enabled_ = {
    kQ1, kO1, kP1,  kS1,  kK1,  kN2, kM2,     kS2,
    kK2, kM4, kMS4, k2N2, kMu2, kJ1, kSigma1, kOO1};

TEST_F(TideTableTest, TableSize) {
  EXPECT_EQ(table.size(), kNumConstituentItems);
  EXPECT_EQ(table.size(),
            static_cast<std::size_t>(Constituent::kNumConstituents));
}

TEST_F(TideTableTest, AllConstituentsPresent) {
  auto keys_vector = table.keys_vector();

  // Check that all constituent values from 0 to kNumConstituents-1 are present
  std::vector<bool> found(kNumConstituentItems, false);
  for (auto constituent : keys_vector) {
    std::size_t index = static_cast<std::size_t>(constituent);
    ASSERT_LT(index, kNumConstituentItems);
    found[index] = true;
  }

  for (std::size_t i = 0; i < kNumConstituentItems; ++i) {
    EXPECT_TRUE(found[i]) << "Constituent " << i << " not found in table";
  }
}

TEST_F(TideTableTest, InitialValues) {
  // All tide table values should be initialized to (0, 0)
  for (std::size_t i = 0; i < kNumConstituentItems; ++i) {
    Constituent constituent = static_cast<Constituent>(i);
    const auto& component = table[constituent];
    EXPECT_EQ(component.tide.real(), 0.0)
        << "Real part should be 0 for constituent " << i;
    EXPECT_EQ(component.tide.imag(), 0.0)
        << "Imaginary part should be 0 for constituent " << i;
    EXPECT_EQ(component.is_inferred, !this->is_enabled(constituent))
        << "Inferred status should match for constituent " << i;
  }
}

TEST_F(TideTableTest, DataConsistency) {
  // Verify that keys match the constituent indices
  const auto& keys = table.keys();
  for (std::size_t i = 0; i < kNumConstituentItems; ++i) {
    Constituent constituent = keys[i];
    std::size_t constituent_index = static_cast<std::size_t>(constituent);
    EXPECT_EQ(constituent_index, i)
        << "Key at position " << i << " should match its constituent value";
  }
}

TEST_F(TideTableTest, Modifiability) {
  // Test that we can modify tide table values
  table[Constituent::kM2].tide = {1.5, 2.3};
  table[Constituent::kS2].tide = {-0.7, 1.2};

  EXPECT_EQ(table[Constituent::kM2].tide.real(), 1.5);
  EXPECT_EQ(table[Constituent::kM2].tide.imag(), 2.3);
  EXPECT_EQ(table[Constituent::kS2].tide.real(), -0.7);
  EXPECT_EQ(table[Constituent::kS2].tide.imag(), 1.2);
}

}  // namespace perth
