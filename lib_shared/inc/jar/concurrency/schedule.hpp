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
#include <functional>
#include <future>
#include <memory>

#include <jar/concurrency/scheduler_type_traits.hpp>

namespace jar::concurrency {
namespace details {

template <typename Value> class receiver {
public:
  receiver()
    : m_value{std::make_shared<std::promise<Value>>()}
  {
  }

  void complete(Value&& value) { m_value->set_value(std::forward<Value>(value)); }

  void fail() noexcept { m_value->set_exception(std::current_exception()); }

  void cancel() noexcept { m_value = nullptr; }

  auto get_future() { return m_value->get_future(); }

private:
  std::shared_ptr<std::promise<Value>> m_value;
};

template <typename Receiver, typename Scheduler> class state {
public:
  state(Receiver&& receiver, Scheduler&& scheduler) noexcept
    : m_receiver{std::forward<Receiver>(receiver)}
    , m_scheduler{std::forward<Scheduler>(scheduler)}
  {
  }

  auto get_future() { return m_receiver.get_future(); }

  void start()
  {
    m_scheduler.schedule([receiver = std::move(m_receiver)]() mutable {
      try {
        receiver.complete();
      } catch (...) {
        receiver.fail();
      }
    });
  }

private:
  Receiver m_receiver;
  Scheduler m_scheduler;
};

template <typename Scheduler> class sender {
public:
  explicit sender(Scheduler&& scheduler) noexcept
    : m_scheduler{std::forward<Scheduler>(scheduler)}
  {
  }

  template <typename Receiver> auto connect(Receiver&& r)
  {
    return state<Receiver, Scheduler>{std::forward<Receiver>(r), std::move(m_scheduler)};
  }

private:
  Scheduler m_scheduler;
};

}  // namespace details

template <typename Scheduler> auto schedule(Scheduler scheduler) noexcept
{
  static_assert(is_output_scheduler<Scheduler>::value, "scheduler must fulfill output Scheduler type requirements");
  return details::sender<Scheduler>{std::forward<Scheduler>(scheduler)};
}

}  // namespace jar::concurrency

#endif  // JAR_CONCURRENCY_SCHEDULE_HPP
