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

#include <jar/util/contract.hpp>

namespace jar::util::test {

using contract = contract<int, -1>;

TEST(contract_test, api_check)
{
  EXPECT_NO_THROW(contract::no_system_error(0));
  EXPECT_THROW(contract::no_system_error(-1), std::system_error);
}

TEST(contract_test, not_null)
{
  static constexpr const std::array<int, 1> value{5};

  EXPECT_NO_THROW(contract::not_null(value.data(), ""));
  EXPECT_THROW(contract::not_null(nullptr, ""), std::invalid_argument);
}

TEST(contract_test, not_zero)
{
  EXPECT_NO_THROW(contract::not_zero(5, ""));
  EXPECT_THROW(contract::not_zero(0, ""), std::invalid_argument);
}

}  // namespace jar::util::test
