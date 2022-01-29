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
/// \file rr_scheduler_test.cpp
///
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <future>
#include <stdexcept>
#include <thread>

#include "jar/concurrency/rr_scheduler.hpp"

namespace jar::concurrency::test {

class mock_task {
public:
  MOCK_METHOD(void, op, (), ());
  void operator()() { op(); }

  MOCK_METHOD(void, op_int, (int), ());
  void operator()(int arg) { op_int(arg); }

  MOCK_METHOD(void, op_args, (int, int), ());
  void operator()(int arg1, int arg2) { op_args(arg1, arg2); }
};

TEST(scheduler_test, test_precondition)
{
  EXPECT_THROW({ rr_scheduler{0U}; }, std::invalid_argument);
}

TEST(scheduler_test, test_scheduling)
{
  static constexpr unsigned task_count{3U};

  rr_scheduler sched{1U};
  mock_task task1, task2, task3;

  EXPECT_CALL(task1, op).Times(1U);
  EXPECT_CALL(task2, op()).Times(1U);
  EXPECT_CALL(task3, op()).Times(1U);

  sched.schedule(std::ref(task1));
  sched.schedule(std::ref(task2));
  sched.schedule(std::ref(task3));

  auto worker = std::async(std::launch::async, [&sched]() {
    for (unsigned n = 0U; n < task_count; ++n) {
      auto task = sched.scheduled();
      task.value()();
    }
  });

  EXPECT_NO_THROW(worker.get());
}

TEST(scheduler_test, test_clear)
{
  rr_scheduler sched{1U};

  auto worker = std::async(std::launch::async, [&sched]() {
    EXPECT_EQ(std::nullopt, sched.scheduled());
  });

  sched.clear();
  EXPECT_NO_THROW(worker.get());
}

TEST(thread_pool_test, test_scheduling_with_adapter)
{
  static constexpr unsigned task_count{3U};
  rr_scheduler sched{1U};

  static constexpr int arg1{1024}, arg2{2048};
  mock_task task1, task2, task3;

  EXPECT_CALL(task1, op).Times(1U);
  EXPECT_CALL(task2, op_int(arg1)).Times(1U);
  EXPECT_CALL(task3, op_args(arg1, arg2)).Times(1U);

  auto adapter = sched.get_adapter();
  EXPECT_NO_THROW(adapter.schedule(std::ref(task1)));
  EXPECT_NO_THROW(adapter.schedule(std::ref(task2), arg1));
  EXPECT_NO_THROW(adapter.schedule(std::ref(task3), arg1, arg2));

  auto worker = std::async(std::launch::async, [&sched]() {
    for (unsigned n = 0U; n < task_count; ++n) {
      auto task = sched.scheduled();
      task.value()();
    }
  });

  EXPECT_NO_THROW(worker.get());
}

}  // namespace jar::concurrency::test
