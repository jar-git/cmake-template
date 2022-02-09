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

#include "jar/concurrency/details/value_receiver.hpp"

#include "jar/concurrency/rr_scheduler.hpp"
#include "jar/concurrency/schedule.hpp"
#include "jar/concurrency/then.hpp"
#include "jar/concurrency/thread_pool.hpp"

namespace jar::concurrency::test {

class then_test : public ::testing::Test {
public:
  static constexpr int s_expected{256};

  thread_pool<rr_scheduler>& executor() { return m_executor; }

private:
  thread_pool<rr_scheduler> m_executor;
};

TEST_F(then_test, test_complete)
{
  auto step1 = then(schedule(executor().get_scheduler()), []() {
    return s_expected;
  });
  auto step2 = then(std::move(step1), [](int arg) {
    EXPECT_EQ(s_expected, arg);
    return std::tuple<int, std::string>{s_expected, std::to_string(arg)};
  });
  auto step3 = then(std::move(step2), [](std::tuple<int, std::string> arg) {
    EXPECT_EQ(s_expected, std::get<int>(arg));
    EXPECT_EQ(std::to_string(s_expected), std::get<std::string>(arg));
    return std::make_unique<int>(std::get<int>(arg));
  });

  auto state = step3.connect(details::value_receiver<std::unique_ptr<int>>{});
  auto future = state.get_future();
  state.start();

  auto value = future.get();
  EXPECT_EQ(s_expected, *value.value());
}

TEST_F(then_test, test_fail)
{
  std::atomic_bool step1_flag{false}, step2_flag{false}, step3_flag{false};

  auto step1 = then(schedule(executor().get_scheduler()), [&step1_flag]() {
    step1_flag.store(true);
    return s_expected;
  });
  auto step2 = then(std::move(step1), [&step2_flag](int) -> std::tuple<int, std::string> {
    step2_flag.store(true);
    throw std::runtime_error{"expected test error"};
  });
  auto step3 = then(std::move(step2), [&step3_flag](std::tuple<int, std::string> arg) {
    step3_flag.store(true);
    return std::make_unique<int>(std::get<int>(arg));
  });

  auto state = step3.connect(details::value_receiver<std::unique_ptr<int>>{});
  auto future = state.get_future();
  state.start();

  EXPECT_THROW(future.get(), std::runtime_error);
  EXPECT_TRUE(step1_flag.load());
  EXPECT_TRUE(step2_flag.load());
  EXPECT_FALSE(step3_flag.load());
}

TEST_F(then_test, test_cancel)
{
  auto step1 = then(schedule(executor().get_scheduler()), []() -> int {
    ADD_FAILURE() << "Canceled 'then' executed!";
  });
  auto step2 = then(std::move(step1), [](int arg) ->  std::tuple<int, std::string> {
    ADD_FAILURE() << "Canceled 'then' executed!";
  });
  auto step3 = then(std::move(step2), [](std::tuple<int, std::string> arg) -> std::unique_ptr<int> {
    ADD_FAILURE() << "Canceled 'then' executed!";
  });

  auto state = step3.connect(details::value_receiver<std::unique_ptr<int>>{});
  auto future = state.get_future();
  future.cancel();
  state.start();

  auto value = future.get();
  EXPECT_TRUE(value.is_canceled());
}

}  // namespace jar::concurrency::test
