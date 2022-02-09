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
/// \file future_test.cpp
///
#include <gtest/gtest.h>

#include <future>
#include <thread>

#include "jar/concurrency/future.hpp"

namespace jar::concurrency::test {

TEST(future_test, test_future_validity)
{
  {
    future<int> future;
    EXPECT_FALSE(future.is_valid());
  }

  promise<int> promise;
  auto future = promise.get_future();
  EXPECT_TRUE(future.is_valid());
}

TEST(future_test, test_set_value)
{
  static constexpr int expected{42};

  promise<int> promise;
  auto future = promise.get_future();

  promise.set_value(expected);
  EXPECT_EQ(expected, future.get());
}

TEST(future_test, test_wait_value)
{
  promise<void> promise;
  auto future = promise.get_future();

  auto trigger = std::async(std::launch::async, [promise = std::move(promise)]() mutable {
    // TODO(jar-git): Fix flaky test (coverage wise), it's not guaranteed that wait actually has to wait. Fixing this
    //  would require some changes to the promise - future pair itself, like a locking strategy type.
    std::this_thread::sleep_for(std::chrono::milliseconds{100U});
    promise.set_value();
  });

  EXPECT_NO_THROW(future.wait());
  EXPECT_TRUE(future.get());
  EXPECT_NO_THROW(trigger.get());
}

TEST(future_test, test_cancel)
{
  auto canceled_future_maker = [](auto canceler) {
    promise<int> promise;
    auto future = promise.get_future();
    canceler(promise, future);
    EXPECT_TRUE(promise.is_canceled());
    promise.set_value(0);
    EXPECT_TRUE(promise.is_canceled());
    return future;
  };

  auto canceled = canceled_future_maker([](auto&, auto& future) {
    future.cancel();
  });
  EXPECT_TRUE(canceled.get().is_canceled());

  auto canceled_by_promise = canceled_future_maker([](auto& promise, auto&) {
    promise.cancel();
  });
  EXPECT_TRUE(canceled_by_promise.get().is_canceled());
}

TEST(future_test, test_exception)
{
  auto throwing_future_maker = []() {
    promise<int> promise;
    auto future = promise.get_future();

    try {
      throw std::runtime_error{"test exception"};
    } catch (...) {
      promise.set_exception(std::current_exception());
    }
    return future;
  };

  auto future = throwing_future_maker();
  EXPECT_THROW(future.get(), std::runtime_error);
}

TEST(future_test, test_broken)
{
  auto broken_future_maker = []() {
    promise<int> promise;
    return promise.get_future();
  };

  auto future = broken_future_maker();
  EXPECT_THROW(future.get(), std::domain_error);
}

}  // namespace jar::concurrency::test
