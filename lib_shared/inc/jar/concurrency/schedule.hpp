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
/// \file schedule.hpp
///

#ifndef JAR_CONCURRENCY_SCHEDULE_HPP
#define JAR_CONCURRENCY_SCHEDULE_HPP

#include <atomic>
#include <exception>
#include <functional>
#include <future>
#include <memory>
#include <system_error>

#include <jar/concurrency/type_traits.hpp>

namespace jar::concurrency {
namespace details {

template <typename Receiver, typename Scheduler> class schedule_state {
public:
  schedule_state(Receiver&& receiver, Scheduler&& scheduler) noexcept
    : m_receiver{std::move(receiver)}
    , m_scheduler{std::move(scheduler)}
  {
  }

  void start()
  {
    m_scheduler.schedule([state = std::move(*this)]() mutable {
      if (!state.m_receiver.is_canceled()) {
        try {
          state.m_receiver.complete();
        } catch (...) {
          state.m_receiver.fail(std::current_exception());
        }
      }
    });
  }

private:
  Receiver m_receiver;
  Scheduler m_scheduler;
};

template <typename Scheduler> class schedule_sender {
public:
  explicit schedule_sender(Scheduler&& scheduler) noexcept
    : m_scheduler{std::move(scheduler)}
  {
  }

  template <typename Receiver> auto connect(Receiver&& receiver)
  {
    return schedule_state<Receiver, Scheduler>{std::forward<Receiver>(receiver), std::move(m_scheduler)};
  }

private:
  Scheduler m_scheduler;
};

}  // namespace details

template <typename Scheduler> auto schedule(Scheduler scheduler) noexcept
{
  static_assert(is_output_scheduler<Scheduler>::value, "scheduler must fulfill output Scheduler type requirements");
  return details::schedule_sender<Scheduler>{std::forward<Scheduler>(scheduler)};
}

}  // namespace jar::concurrency

#endif  // JAR_CONCURRENCY_SCHEDULE_HPP
