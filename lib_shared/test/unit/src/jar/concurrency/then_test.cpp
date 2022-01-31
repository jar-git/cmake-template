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
/// \file schedule_test.cpp
///
#include <gtest/gtest.h>

#include <string>
#include <tuple>
#include <future>

#include "jar/concurrency/rr_scheduler.hpp"
#include "jar/concurrency/schedule.hpp"
#include "jar/concurrency/then.hpp"
#include "jar/concurrency/thread_pool.hpp"

namespace jar::concurrency::test {

TEST(then_test, test_complete)
{
  thread_pool<rr_scheduler> executor;

  auto first = then(schedule(executor.get_scheduler()), []() {
    return 42;
  });
  auto after = then(std::move(first), [](int arg) {
    return std::tuple<int, std::string>{arg, std::to_string(arg)};
  });

  auto state = after.connect(details::receiver<std::tuple<int, std::string>>{});
  auto future = state.get_future();
  state.start();

  EXPECT_NO_THROW(future.wait());
  auto [i, s] = future.get();
  EXPECT_EQ(42, i);
  EXPECT_EQ("42", s);
}

TEST(then_test, test_fail) { }

TEST(then_test, test_cancel) { }

}  // namespace jar::concurrency::test
