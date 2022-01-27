/// Copyright 2022 Jani Arola, All rights reserved.
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
/// \file latch_test.cpp
///
#include <gtest/gtest.h>

#include <future>
#include <limits>

#include "jar/concurrency/latch.hpp"

namespace jar::concurrency::test {

TEST(latch_test, test_try_waiting_and_countdown)
{
  latch one{1U};

  EXPECT_FALSE(one.try_wait());
  one.count_down();
  EXPECT_TRUE(one.try_wait());

  latch max{std::numeric_limits<std::size_t>::max()};
  EXPECT_FALSE(max.try_wait());
  max.count_down(std::numeric_limits<std::size_t>::max());
  EXPECT_TRUE(max.try_wait());
}

TEST(latch_test, test_arrive_and_wait)
{
  latch one{1U};
  EXPECT_NO_THROW(one.arrive_and_wait());

  latch max{std::numeric_limits<std::size_t>::max()};
  EXPECT_NO_THROW(max.arrive_and_wait(std::numeric_limits<std::size_t>::max()));
}

TEST(latch_test, test_wait_and_countdown)
{
  latch one{1U};
  latch max{std::numeric_limits<std::size_t>::max()};

  auto count_down = std::async(std::launch::async, [&one, &max]() {
    one.count_down();
    max.count_down(std::numeric_limits<std::size_t>::max());
  });

  EXPECT_NO_THROW(one.wait());
  EXPECT_NO_THROW(max.wait());
  EXPECT_NO_THROW(count_down.get());
}

}  // namespace jar::async::test
