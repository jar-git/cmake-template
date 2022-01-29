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
#include <tuple>
#include <vector>

namespace jar::concurrency {

template <typename Scheduler> class thread_pool {
  class scheduler_adapter {
  public:
    explicit scheduler_adapter(std::reference_wrapper<Scheduler> scheduler)
      : m_scheduler{scheduler}
    {
    }

    template <typename Invocable, typename... Args> void schedule(Invocable&& invocable, Args&&... args)
    {
      static_assert(std::is_invocable_v<Invocable, Args...>, "Invocable type must be invocable with args");
      m_scheduler.get().schedule(
          [invocable = std::move(invocable), args = std::tuple(std::forward<Args>(args)...)]() mutable {
            return std::apply(
                [&invocable](auto&&... args) {
                  std::invoke(invocable, args...);
                },
                std::move(args));
          });
    }

  private:
    std::reference_wrapper<Scheduler> m_scheduler;
  };

public:
  explicit thread_pool(unsigned thread_count = std::thread::hardware_concurrency())
    : m_thread_count{std::max(1U, thread_count)}
    , m_threads{m_thread_count}
    , m_scheduler{m_thread_count}
  {
    for (auto& thread : m_threads) {
      thread = std::thread{[this]() {
        run();
      }};
    }
  }

  thread_pool(const thread_pool&) = delete;
  thread_pool(thread_pool&&) = delete;
  thread_pool& operator=(const thread_pool&) = delete;
  thread_pool& operator=(thread_pool&&) = delete;

  ~thread_pool()
  {
    m_scheduler.clear();

    for (auto& thread : m_threads) {
      thread.join();
    }
  }

  auto get_scheduler() noexcept { return scheduler_adapter{std::ref(m_scheduler)}; }

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

  unsigned const m_thread_count;
  std::vector<std::thread> m_threads;
  Scheduler m_scheduler;
};

}  // namespace jar::concurrency

#endif  // JAR_CONCURRENCY_THREAD_POOL_HPP
