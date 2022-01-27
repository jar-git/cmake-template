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
/// \file latch.hpp
///

#ifndef JAR_CONCURRENCY_LATCH_HPP
#define JAR_CONCURRENCY_LATCH_HPP

#include <atomic>
#include <condition_variable>
#include <cstddef>
#include <mutex>

namespace jar::concurrency {

class latch {
public:
  explicit latch(std::size_t expected) noexcept;

  latch(latch const&) = delete;
  latch& operator=(latch const&) = delete;
  latch(latch&&) = delete;
  latch& operator=(latch&&) = delete;

  void count_down(std::size_t n = std::size_t{1U});

  void wait() const;

  bool try_wait() const noexcept { return s_destination == m_expected.load(std::memory_order_relaxed); }

  void arrive_and_wait(std::size_t n = std::size_t{1});

private:
  inline constexpr static std::size_t s_destination{0};

  std::atomic_size_t m_expected;
  mutable std::condition_variable m_condition;
  mutable std::mutex m_mutex;
};

}  // namespace jar::async

#endif  // JAR_CONCURRENCY_LATCH_HPP
