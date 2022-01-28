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
/// \file scheduler.cpp
///
#include "jar/concurrency/scheduler.hpp"

#include <jar/core/contract.hpp>

namespace jar::concurrency {

scheduler::scheduler(unsigned queue_count)
  : m_task_queue{queue_count}
  , m_push_index{0U}
  , m_pop_index{0U}
{
  contract::not_zero(queue_count, "queue_count cannot be zero");
}

std::optional<scheduler::task_type> scheduler::scheduled()
{
  static thread_local unsigned const thread_index{m_pop_index.fetch_add(1U, std::memory_order_relaxed)};

  std::optional<task_type> task;
  for (unsigned n = 0U; n != m_task_queue.size(); ++n) {
    task = m_task_queue[(thread_index + n) % m_task_queue.size()].try_pop();
    if (task.has_value()) {
      return task;
    }
  }

  return m_task_queue[thread_index % m_task_queue.size()].pop();
}

void scheduler::clear() noexcept
{
  for (auto& queue : m_task_queue) {
    queue.clear();
  }
}

void scheduler::schedule(task_type&& task)
{
  const std::size_t try_n_times{m_task_queue.size() * 4U};

  auto index = m_push_index.fetch_add(1U, std::memory_order_relaxed);
  for (unsigned n = 0U; n != try_n_times; ++n) {
    if (m_task_queue[(index + n) % m_task_queue.size()].try_push(task)) {
      return;
    }
  }

  m_task_queue[index % m_task_queue.size()].push(std::forward<task_type>(task));
}

}  // namespace jar::concurrency
