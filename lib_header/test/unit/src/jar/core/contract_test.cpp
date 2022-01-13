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
/// \file contract_test.cpp
///
#include <gtest/gtest.h>

#include <functional>

#include "jar/core/contract.hpp"

namespace jar::contract::test {

TEST(contract_test, test_no_system_error)
{
  EXPECT_NO_THROW(no_system_error(0));
  EXPECT_THROW(no_system_error(-1), std::system_error);
  EXPECT_THROW((no_system_error<long long, 0>(0)), std::system_error);
  EXPECT_NO_THROW((no_system_error_other_than(-1, 0)));
  EXPECT_THROW((no_system_error_other_than(-1, ENOENT)), std::system_error);
}

TEST(contract_test, test_not_null)
{
  {
    static constexpr const std::array<int, 1> buffer{5};
    std::function<void(void)> empty_function = []() {};
    auto int_ptr = std::make_unique<int>(5);

    EXPECT_NO_THROW(not_null(buffer.data(), ""));
    EXPECT_NO_THROW(not_null(empty_function, ""));
    EXPECT_NO_THROW(not_null(int_ptr, ""));
  }

  {
    static constexpr int const* const null_ptr{nullptr};
    std::function<void(void)> null_function{nullptr};
    std::unique_ptr<int> null_unique_ptr{nullptr};

    EXPECT_THROW(not_null(null_ptr, ""), std::invalid_argument);
    EXPECT_THROW(not_null(null_function, ""), std::invalid_argument);
    EXPECT_THROW(not_null(null_unique_ptr, ""), std::invalid_argument);
  }
}

TEST(contract_test, test_not_zero)
{
  EXPECT_NO_THROW(not_zero(5, ""));
  EXPECT_THROW(not_zero(0, ""), std::invalid_argument);
}

TEST(contract_test, test_not_greater)
{
  static constexpr int max{5};

  EXPECT_NO_THROW(not_greater(5, max, ""));
  EXPECT_NO_THROW(not_greater(4, max, ""));
  EXPECT_THROW(not_greater(6, max, ""), std::invalid_argument);
}

}  // namespace jar::contract::test
