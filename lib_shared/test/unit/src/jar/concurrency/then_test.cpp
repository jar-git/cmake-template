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

#include <future>
#include <memory>
#include <string>
#include <tuple>

#include "jar/concurrency/rr_scheduler.hpp"
#include "jar/concurrency/schedule.hpp"
#include "jar/concurrency/then.hpp"
#include "jar/concurrency/thread_pool.hpp"

namespace jar::concurrency::test {

TEST(then_test, test_complete)
{
  thread_pool<rr_scheduler> executor;

  auto step1 = then(schedule(executor.get_scheduler()), []() {
    return 42;
  });
  auto step2 = then(std::move(step1), [](int arg) {
    EXPECT_EQ(42, arg);
    return std::tuple<int, std::string>{256, std::to_string(arg)};
  });
  auto final = then(std::move(step2), [](std::tuple<int, std::string> arg) {
    EXPECT_EQ(256, std::get<int>(arg));
    EXPECT_EQ("42", std::get<std::string>(arg));
    return std::make_unique<int>(std::get<int>(arg) * 2);
  });

  auto state = final.connect(utilities::value_receiver<std::unique_ptr<int>>{});
  auto future = state.get_future();
  state.start();

  EXPECT_EQ(512, *future.get());
}

TEST(then_test, test_fail) { thread_pool<rr_scheduler> executor; }

TEST(then_test, test_cancel) { }

}  // namespace jar::concurrency::test
