/// Copyright 2020 Jani Arola, All rights reserved.
///
/// Licensed under the Apache License, Version 2.0 (the "License");
/// you may not use this file except in compliance with the License.
/// You may obtain a copy of the License at
///
///     http://www.apache.org/licenses/LICENSE-2.0
///
/// Unless required by applicable law or agreed to in writing, software
/// distributed under the License is distributed on an "AS IS" BASIS,
/// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
/// See the License for the specific language governing permissions and
/// limitations under the License.
///
/// \file handle_test.cpp
///
#include <gtest/gtest.h>

#include <cassert>

#include "jar/system/basic_handle.hpp"

namespace jar::system::test {

/// \brief Dummy resource for testing
class dummy_resource {
public:
  /// \brief handle for lifetime testing
  static constexpr std::int32_t s_lifetime_handle{1337};

  /// \brief Implement NativeType concept
  using native_type = std::int32_t;

  /// \brief Implement invalid handle concept
  [[nodiscard]] constexpr static native_type invalid_value() { return native_type{-1}; }

  /// \brief Implement construction concept
  template <typename... Args> constexpr static auto construct(Args&&... args)
  {
    return std::get<0>(std::forward_as_tuple(args...));
  }

  /// \brief Implement destroy concept
  constexpr static void destroy(native_type handle) noexcept { assert(s_lifetime_handle != handle); }
};

/// \brief Dummy handle instance for testing
class dummy_handle : public basic_handle<dummy_resource> {
public:
  /// \brief Constructor
  constexpr dummy_handle(std::int32_t value)
    : basic_handle<dummy_resource>{value}
  {
  }

  /// \brief Conversion from protected to public operation for testing
  [[nodiscard]] constexpr std::int32_t to_integral() const noexcept
  {
    return basic_handle<dummy_resource>::operator native_type();
  }
};

TEST(basic_handle_test, test_lifetime)
{
  EXPECT_DEATH({ dummy_handle{dummy_resource::s_lifetime_handle}; }, "s_lifetime_handle != handle");
}

TEST(basic_handle_test, test_conversion)
{
  constexpr std::int32_t expected_value{1};
  dummy_handle handle{expected_value};
  EXPECT_EQ(expected_value, handle.to_integral());
}

TEST(basic_handle_test, test_comparison)
{
  dummy_handle lh_value{1};
  dummy_handle rh_value{2};

  EXPECT_EQ(lh_value, lh_value);
  EXPECT_NE(lh_value, rh_value);
}

TEST(basic_handle_test, test_move)
{
  constexpr std::int32_t expected_value{1};

  dummy_handle original{expected_value};
  EXPECT_TRUE(original.is_valid());

  dummy_handle moved{std::move(original)};
  EXPECT_TRUE(moved.is_valid());
  EXPECT_FALSE(original.is_valid());
  EXPECT_EQ(expected_value, moved.to_integral());
}

}  // namespace jar::system::test
