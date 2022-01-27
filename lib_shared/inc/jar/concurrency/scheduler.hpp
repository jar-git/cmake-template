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
/// \file scheduler.hpp
///

#ifndef JAR_ASYNC_SCHEDULER_HPP
#define JAR_ASYNC_SCHEDULER_HPP

#include <atomic>
#include <functional>
#include <optional>
#include <tuple>
#include <vector>

#include <jar/concurrency/queue.hpp>

namespace jar::concurrency {

class scheduler {
public:
  using task_type = std::function<void(void)>;

  explicit scheduler(unsigned queue_count);

  std::optional<task_type> scheduled();

  void clear() noexcept;

  template <typename Invocable, typename... Args> void schedule(Invocable&& invocable, Args&&... args)
  {
    static_assert(std::is_invocable_v<Invocable, Args...>, "Invocable type must be invocable with args");
    schedule_rr([invocable = std::move(invocable), args = std::tuple(std::forward<Args>(args)...)]() mutable {
      return std::apply(
          [&invocable](auto&&... args) {
            std::invoke(invocable, args...);
          },
          std::move(args));
    });
  }

private:
  using task_queue = std::vector<queue<task_type>>;

  void schedule_rr(task_type&& task);

  task_queue m_task_queue;
  std::atomic_uint m_push_index;
  std::atomic_uint m_pop_index;
};

}  // namespace jar::concurrency

#endif  // JAR_ASYNC_SCHEDULER_HPP
