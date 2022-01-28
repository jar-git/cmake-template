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

#ifndef JAR_CONCURRENCY_SCHEDULER_HPP
#define JAR_CONCURRENCY_SCHEDULER_HPP

#include <atomic>
#include <functional>
#include <optional>
#include <vector>

#include <jar/concurrency/queue.hpp>

namespace jar::concurrency {

class scheduler {
public:
  using task_type = std::function<void(void)>;

  explicit scheduler(unsigned queue_count);

  std::optional<task_type> scheduled();

  void schedule(task_type&& task);

  void clear() noexcept;

private:
  using task_queue = std::vector<queue<task_type>>;

  task_queue m_task_queue;
  std::atomic_uint m_push_index;
  std::atomic_uint m_pop_index;
};

}  // namespace jar::concurrency

#endif  // JAR_CONCURRENCY_SCHEDULER_HPP
