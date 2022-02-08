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
/// \file value_receiver.hpp
///

#ifndef JAR_CONCURRENCY_DETAILS_VALUE_RECEIVER_HPP
#define JAR_CONCURRENCY_DETAILS_VALUE_RECEIVER_HPP

#include <atomic>
#include <cassert>
#include <future>
#include <memory>
#include <optional>

namespace jar::concurrency::details {

template <typename... Value> class value_receiver {
  enum class value_state { initial, completed, failed, canceled };

public:
  value_receiver()
    : m_value{std::make_shared<std::promise<std::optional<Value...>>>()}
    , m_state{value_state::initial}
  {
  }

  value_receiver(value_receiver const& other)
    : m_value{other.m_value}
    , m_state{other.m_state.load()}
  {
    assert(value_state::initial == m_state.load());
  }

  ~value_receiver() = default;

  void complete(Value&&... value)
  {
    auto initial_state = value_state::initial;
    if (m_state.compare_exchange_strong(initial_state, value_state::completed)) {
      m_value->set_value(std::forward<Value>(value)...);
    }
  }

  void fail() noexcept
  {
    auto initial_state = value_state::initial;
    if (m_state.compare_exchange_strong(initial_state, value_state::failed)) {
      m_value->set_exception(std::current_exception());
    }
  }

  void cancel() noexcept
  {
    auto initial_state = value_state::initial;
    if (m_state.compare_exchange_strong(initial_state, value_state::canceled)) {
      m_value->set_value(std::nullopt);
    }
  }

  bool is_canceled() noexcept { return value_state::canceled == m_state.load(); }

  auto get_future() { return m_value->get_future(); }

private:
  std::shared_ptr<std::promise<std::optional<Value...>>> m_value;
  std::atomic<value_state> m_state;
};

}  // namespace jar::concurrency::details

#endif  // JAR_CONCURRENCY_DETAILS_VALUE_RECEIVER_HPP
