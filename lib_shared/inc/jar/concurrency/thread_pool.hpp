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
/// \file thread_pool.hpp
///

#ifndef JAR_CONCURRENCY_THREAD_POOL_HPP
#define JAR_CONCURRENCY_THREAD_POOL_HPP

#include <algorithm>
#include <optional>
#include <thread>
#include <vector>

#include <jar/concurrency/scheduler_type_traits.hpp>

namespace jar::concurrency {

template <typename Scheduler> class thread_pool {
public:
  explicit thread_pool(unsigned thread_count = std::thread::hardware_concurrency())
    : m_thread_count{std::max(1U, thread_count)}
    , m_threads{m_thread_count}
    , m_scheduler{m_thread_count}
  {
    static_assert(is_input_scheduler<Scheduler>::value, "scheduler must fulfill input Scheduler type requirements");

    try {
      for (auto& thread : m_threads) {
        thread = std::thread{[this]() {
          run();
        }};
      }
    } catch (...) {
      join();
      throw;
    }
  }

  thread_pool(const thread_pool&) = delete;
  thread_pool(thread_pool&&) = delete;
  thread_pool& operator=(const thread_pool&) = delete;
  thread_pool& operator=(thread_pool&&) = delete;

  ~thread_pool() { join(); }

  auto get_scheduler()
  {
    if constexpr (has_scheduler_adapter<Scheduler>::value) {
      return m_scheduler.get_adapter();
    } else {
      return m_scheduler;
    }
  }

private:
  void run() noexcept
  {
    using task_type = typename Scheduler::task_type;

    std::optional<task_type> task;
    do {
      task = m_scheduler.scheduled();

      if (task.has_value()) {
        task.value()();
      }
    } while (task.has_value());
  }

  void join() noexcept
  {
    m_scheduler.clear();

    for (auto& thread : m_threads) {
      if (thread.joinable()) {
        thread.join();
      }
    }
  }

  unsigned const m_thread_count;
  std::vector<std::thread> m_threads;
  Scheduler m_scheduler;
};

}  // namespace jar::concurrency

#endif  // JAR_CONCURRENCY_THREAD_POOL_HPP
