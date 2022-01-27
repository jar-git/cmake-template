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
/// \file queue.hpp
///

#ifndef JAR_CONCURRENCY_QUEUE_HPP
#define JAR_CONCURRENCY_QUEUE_HPP

#include <condition_variable>
#include <deque>
#include <mutex>
#include <optional>

namespace jar::concurrency {

template <typename T, typename Container = std::deque<T>> class queue {
public:
  queue()
    : m_is_cancelled{false}
    , m_container{}
  {
  }

  std::optional<T> pop() noexcept(std::is_nothrow_move_constructible_v<T>)
  {
    std::unique_lock<std::mutex> lock{m_mutex};
    if (m_container.empty() || !m_is_cancelled) {
      m_condition.wait(lock, [this]() {
        return !m_container.empty() || m_is_cancelled;
      });
    }

    if (m_is_cancelled) {
      return std::nullopt;
    }

    return pop_front();
  }

  std::optional<T> try_pop() noexcept(std::is_nothrow_move_constructible_v<T>)
  {
    std::unique_lock<std::mutex> lock{m_mutex, std::try_to_lock};
    if (m_container.empty() || !lock) {
      return std::nullopt;
    }

    return pop_front();
  }

  void push(T item)
  {
    {
      std::lock_guard<std::mutex> lock{m_mutex};
      m_container.emplace_back(std::forward<T>(item));
    }
    m_condition.notify_one();
  }

  bool try_push(T item)
  {
    {
      std::unique_lock<std::mutex> lock{m_mutex, std::try_to_lock};
      if (!lock) {
        return false;
      }
      m_container.emplace_back(std::forward<T>(item));
    }
    m_condition.notify_one();
    return true;
  }

  void clear() noexcept
  {
    {
      std::lock_guard<std::mutex> lock{m_mutex};
      m_is_cancelled = true;
      m_container.clear();
    }
    m_condition.notify_all();
  }

private:
  std::optional<T> pop_front()
  {
    auto item = std::move(m_container.front());
    m_container.pop_front();
    return item;
  }

  bool m_is_cancelled;
  Container m_container;
  std::mutex m_mutex;
  std::condition_variable m_condition;
};

}  // namespace jar::concurrency

#endif  // JAR_CONCURRENCY_QUEUE_HPP
