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
/// \file queue_test.cpp
///
#include <gtest/gtest.h>

#include <future>
#include <memory>

#include "jar/concurrency/queue.hpp"

namespace jar::concurrency::test {

static constexpr int g_item_count = 100;

TEST(queue_test, test_throughput)
{
  queue<std::unique_ptr<int>> ptr_queue;

  auto producer = std::async(std::launch::async, [&ptr_queue]() {
    for (int i = 0; i < g_item_count; ++i) {
      ptr_queue.push(std::make_unique<int>(i));
    }
  });

  auto consumer = [](queue<std::unique_ptr<int>>& ptr_queue) {
    for (int i = 0; i < g_item_count; ++i) {
      EXPECT_EQ(i, *ptr_queue.pop().value());
    }
  };
  consumer(ptr_queue);
}

TEST(queue_test, test_try_throughput)
{
  queue<int> int_queue;

  auto producer = std::async(std::launch::async, [&int_queue]() {
    for (int i = 0; i < g_item_count;) {
      bool has_pushed = int_queue.try_push(i);
      if (has_pushed) {
        ++i;
      }
      std::this_thread::yield();
    }
  });

  auto consumer = [](queue<int>& int_queue) {
    for (int i = 0; i < g_item_count;) {
      auto value = int_queue.try_pop();
      if (value) {
        EXPECT_EQ(i, value);
        ++i;
      }
      std::this_thread::yield();
    }
  };
  consumer(int_queue);
}

TEST(queue_test, test_try_pop_empty)
{
  queue<int> queue;
  EXPECT_EQ(std::nullopt, queue.try_pop());
}

TEST(queue_test, test_clear)
{
  queue<int> queue;

  auto consumer = std::async(std::launch::async, [&queue]() {
    EXPECT_EQ(std::nullopt, queue.pop());
  });

  queue.clear();
}

}  // namespace jar::concurrency::test
