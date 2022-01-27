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
/// \file latch.cpp
///
#include "jar/concurrency/latch.hpp"

#include <limits>

namespace jar::concurrency {

latch::latch(std::size_t expected) noexcept
  : m_expected{expected}
{
}

void latch::count_down(std::size_t n)
{
  m_expected -= n;
  if (s_destination == m_expected) {
    m_condition.notify_all();
  }
}

void latch::wait() const
{
  std::unique_lock<std::mutex> lock{m_mutex};
  if(s_destination != m_expected.load(std::memory_order_relaxed)) {
    m_condition.wait(lock, [this] {
      return s_destination == m_expected.load(std::memory_order_relaxed);
    });
  }
}

void latch::arrive_and_wait(std::size_t n)
{
  count_down(n);
  wait();
}

}  // namespace jar::async
