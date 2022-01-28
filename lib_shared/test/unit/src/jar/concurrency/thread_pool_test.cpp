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
/// \file thread_pool_test.cpp
///
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <condition_variable>
#include <mutex>
#include <optional>

#include "jar/concurrency/thread_pool.hpp"

using ::testing::Return;

namespace jar::concurrency::test {

class mock_scheduler {
public:
  using task_type = std::function<void(void)>;

  class singleton {
  public:
    MOCK_METHOD(void, constructor, (unsigned), ());
    MOCK_METHOD(void, schedule, (task_type), ());
    MOCK_METHOD(std::optional<task_type>, scheduled, ());
    MOCK_METHOD(void, clear, (), (noexcept));

    static singleton& get_instance() noexcept
    {
      static singleton instance;
      return instance;
    }

  private:
    singleton() { ON_CALL(*this, scheduled()).WillByDefault(Return(std::optional<task_type>{std::nullopt})); }
    ~singleton() = default;
  };

  explicit mock_scheduler(unsigned thread_count)
  {
    auto& instance = singleton::get_instance();
    instance.constructor(thread_count);
  }

  void schedule(task_type task)
  {
    auto& instance = singleton::get_instance();
    instance.schedule(std::forward<task_type>(task));
  }

  std::optional<task_type> scheduled()
  {
    auto& instance = singleton::get_instance();
    return instance.scheduled();
  }

  void clear() noexcept
  {
    auto& instance = singleton::get_instance();
    instance.clear();
  }
};

TEST(thread_pool_test, test_lifetime)
{
  auto& instance = mock_scheduler::singleton::get_instance();
  auto const default_thread_count = std::thread::hardware_concurrency();

  EXPECT_CALL(instance, constructor(default_thread_count)).Times(1U);
  EXPECT_CALL(instance, scheduled()).Times(default_thread_count);
  EXPECT_CALL(instance, clear()).Times(1U);
  {
    thread_pool<mock_scheduler> thread_pool;
  }

  EXPECT_CALL(instance, constructor(1U)).Times(1U);
  EXPECT_CALL(instance, scheduled()).Times(1U);
  EXPECT_CALL(instance, clear()).Times(1U);
  {
    thread_pool<mock_scheduler> thread_pool{1U};
  }

  EXPECT_CALL(instance, constructor(1U)).Times(1U);
  EXPECT_CALL(instance, scheduled()).Times(1U);
  EXPECT_CALL(instance, clear()).Times(1U);
  {
    thread_pool<mock_scheduler> thread_pool{0U};
  }
}

TEST(thread_pool_test, test_execution)
{
  static constexpr unsigned thread_count{4U};
  static constexpr auto task_count = thread_count;
  static constexpr std::chrono::milliseconds task_duration{100U};
  auto& instance = mock_scheduler::singleton::get_instance();

  std::mutex mutex;
  std::condition_variable condition;
  std::atomic_uint count{0U};

  auto task = [&count, &condition]() {
    ++count;
    std::this_thread::sleep_for(task_duration);
    condition.notify_one();
  };

  EXPECT_CALL(instance, constructor(thread_count)).Times(1U);
  EXPECT_CALL(instance, scheduled())
      .Times(thread_count + task_count)
      .WillOnce(Return(task))
      .WillOnce(Return(task))
      .WillOnce(Return(task))
      .WillOnce(Return(task))
      .WillRepeatedly(Return(std::nullopt));
  EXPECT_CALL(instance, clear()).Times(1U);
  {
    thread_pool<mock_scheduler> thread_pool{thread_count};

    auto tasks_completed = [&count]() {
      return task_count == count;
    };
    std::unique_lock<std::mutex> lock{mutex};
    EXPECT_TRUE(condition.wait_for(lock, task_duration * 2, tasks_completed));
  }
}

TEST(thread_pool_test, test_scheduler_adapter)
{
  class mock_task {
  public:
    MOCK_METHOD(void, op, (), ());
    void operator()() { op(); }

    MOCK_METHOD(void, op_int, (int), ());
    void operator()(int arg) { op_int(arg); }

    MOCK_METHOD(void, op_args, (int, int), ());
    void operator()(int arg1, int arg2) { op_args(arg1, arg2); }
  };

  auto& instance = mock_scheduler::singleton::get_instance();

  EXPECT_CALL(instance, constructor(1U)).Times(1U);
  EXPECT_CALL(instance, scheduled()).Times(1U);
  EXPECT_CALL(instance, clear()).Times(1U);
  {
    thread_pool<mock_scheduler> thread_pool{1U};
    auto scheduler = thread_pool.get_scheduler();
    EXPECT_TRUE(std::is_trivially_copyable_v<decltype(scheduler)>);

    static constexpr int arg1{1024}, arg2{2048};
    mock_task task1, task2, task3;

    EXPECT_CALL(task1, op).Times(1U);
    EXPECT_CALL(task2, op_int(arg1)).Times(1U);
    EXPECT_CALL(task3, op_args(arg1, arg2)).Times(1U);

    EXPECT_CALL(instance, schedule(::testing::_))
        .Times(3U)
        .WillOnce([](mock_scheduler::task_type task1) {
          task1();
        })
        .WillOnce([](mock_scheduler::task_type task2) {
          task2();
        })
        .WillOnce([](mock_scheduler::task_type task3) {
          task3();
        });

    EXPECT_NO_THROW(scheduler.schedule(std::ref(task1)));
    EXPECT_NO_THROW(scheduler.schedule(std::ref(task2), arg1));
    EXPECT_NO_THROW(scheduler.schedule(std::ref(task3), arg1, arg2));
  }
}

}  // namespace jar::concurrency::test
