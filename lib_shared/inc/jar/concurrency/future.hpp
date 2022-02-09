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
/// \file future.hpp
///

#ifndef JAR_CONCURRENCY_FUTURE_HPP
#define JAR_CONCURRENCY_FUTURE_HPP

#include <atomic>
#include <condition_variable>
#include <memory>
#include <optional>
#include <type_traits>
#include <utility>
#include <variant>

namespace jar::concurrency {
namespace details {

template <typename Value> class future_result {
public:
  explicit future_result(Value&& value) noexcept(std::is_nothrow_move_constructible_v<Value>)
    : m_value{std::move(value)}
  {
  }

  explicit future_result(Value const& value) noexcept(std::is_nothrow_copy_constructible_v<Value>)
    : m_value{value}
  {
  }

  explicit future_result(std::nullopt_t opt) noexcept
    : m_value{opt}
  {
  }

  operator Value&() { return m_value.value(); }
  operator const Value&() const { return m_value.value(); }
  explicit operator bool() const noexcept { return m_value.has_value(); }

  Value& value() { return m_value.value(); }
  Value const& value() const { return m_value.value(); }

  bool is_canceled() const noexcept { return !m_value.has_value(); }

private:
  std::optional<Value> m_value;
};

template <> class future_result<void> {
public:
  future_result() noexcept
    : m_is_canceled{false}
  {
  }

  explicit future_result(std::nullopt_t) noexcept
    : m_is_canceled{true}
  {
  }

  explicit operator bool() const noexcept { return !is_canceled(); }
  bool is_canceled() const noexcept { return m_is_canceled; }

private:
  bool m_is_canceled;
};

template <typename Value> class shared_state {
  enum class state { init, value, error, canceled, broken };

public:
  void wait() const
  {
    if (!is_ready()) {
      std::unique_lock<std::mutex> lock{m_mutex};
      if (!is_ready()) {
        m_condition.wait(lock, [this] {
          return is_ready();
        });
      }
    }
  }

  future_result<Value> get()
  {
    wait();

    switch (m_state.load(std::memory_order_relaxed)) {
    case state::canceled:
      [[fallthrough]];
    case state::value:
      return std::move(std::get<future_result<Value>>(m_data));
    case state::error:
      std::rethrow_exception(std::get<std::exception_ptr>(m_data));
    case state::broken:
      throw std::domain_error{"broken promise"};
    default:
      throw std::domain_error{"shared state broken"};
    }
  }

  void set_value(future_result<Value>&& value)
  {
    auto initial_state = state::init;
    if (m_state.compare_exchange_strong(initial_state, state::value)) {
      m_data = std::move(value);
      m_condition.notify_one();
    }
  }

  void set_exception(std::exception_ptr e)
  {
    auto initial_state = state::init;
    if (m_state.compare_exchange_strong(initial_state, state::error)) {
      m_data = std::move(e);
      m_condition.notify_one();
    }
  }

  void cancel()
  {
    auto initial_state = state::init;
    if (m_state.compare_exchange_strong(initial_state, state::canceled)) {
      m_data = future_result<Value>(std::nullopt);
      m_condition.notify_one();
    }
  }

  void broken()
  {
    auto initial_state = state::init;
    if (m_state.compare_exchange_strong(initial_state, state::broken)) {
      m_condition.notify_one();
    }
  }

  bool is_canceled() const noexcept { return state::canceled == m_state.load(std::memory_order_relaxed); }

private:
  bool is_ready() const noexcept { return state::init != m_state.load(std::memory_order_relaxed); }

  mutable std::mutex m_mutex;
  mutable std::condition_variable m_condition;

  std::variant<std::monostate, std::exception_ptr, future_result<Value>> m_data;
  std::atomic<state> m_state;
};

}  // namespace details

template <typename Value> class promise;

template <typename Value> class future {
  friend class promise<Value>;

public:
  future() noexcept
    : m_shared_state{nullptr}
  {
  }

  future(future const&) = delete;
  future(future&&) noexcept = default;
  future& operator=(future const&) = delete;
  future& operator=(future&&) noexcept = default;

  ~future() = default;

  auto get() { return m_shared_state->get(); }

  bool is_valid() const noexcept { return nullptr != m_shared_state; }

  void wait() const { return m_shared_state->wait(); }

  void cancel() { m_shared_state->cancel(); }

private:
  explicit future(std::shared_ptr<details::shared_state<Value>> state)
    : m_shared_state(std::move(state))
  {
  }

  std::shared_ptr<details::shared_state<Value>> m_shared_state;
};

template <typename Value> class promise {
public:
  promise()
    : m_shared_state{std::make_shared<details::shared_state<Value>>()}
  {
  }

  promise(promise const&) = delete;
  promise(promise&&) noexcept = default;
  promise& operator=(promise const&) = delete;
  promise& operator=(promise&&) noexcept = default;

  ~promise()
  {
    if (nullptr != m_shared_state) {
      m_shared_state->broken();
    }
  }

  auto get_future() { return future<Value>{m_shared_state}; }

  template <typename V = Value, std::enable_if_t<!std::is_same_v<V, void>, bool> = true> void set_value(V&& value)
  {
    m_shared_state->set_value(details::future_result<Value>(std::forward<V>(value)));
  }

  template <typename T = Value, std::enable_if_t<std::is_same_v<T, void>, bool> = true> void set_value()
  {
    m_shared_state->set_value(details::future_result<Value>{});
  }

  void set_exception(std::exception_ptr e) { m_shared_state->set_exception(std::move(e)); }

  bool is_canceled() const noexcept { return m_shared_state->is_canceled(); }

  void cancel() { m_shared_state->cancel(); }

private:
  std::shared_ptr<details::shared_state<Value>> m_shared_state;
};

}  // namespace jar::concurrency

#endif  // JAR_CONCURRENCY_FUTURE_HPP
