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
/// \file callback_receiver.hpp
///

#ifndef JAR_CONCURRENCY_DETAILS_CALLBACK_RECEIVER_HPP
#define JAR_CONCURRENCY_DETAILS_CALLBACK_RECEIVER_HPP

#include <atomic>
#include <exception>
#include <functional>
#include <stdexcept>
#include <utility>

#include <jar/concurrency/type_traits.hpp>

namespace jar::concurrency::details {

template <typename Value, typename CompleteHandler, typename ErrorHandler, typename CancelHandler>
class callback_receiver {
  enum class receiver_state { initial, completed, failed, canceled };

public:
  callback_receiver(CompleteHandler&& complete, ErrorHandler error, CancelHandler cancel)
    : m_state{receiver_state::initial}
    , m_complete_handler{std::move(complete)}
    , m_error_handler{std::move(error)}
    , m_cancel_handler{std::move(cancel)}
  {
    static_assert(std::is_invocable_v<ErrorHandler, std::exception_ptr>,
                  "ErrorHandler must be noexcept and must accept std::exception_ptr as argument");
    static_assert(std::is_invocable_v<CancelHandler>,
                  "CancelHandler must be noexcept and must not expect any arguments");
  }

  callback_receiver(callback_receiver&& other)
    : m_state{other.m_state.load()}
    , m_complete_handler{std::move(other.m_complete_handler)}
    , m_error_handler{std::move(other.m_error_handler)}
    , m_cancel_handler{std::move(other.m_cancel_handler)}
  {
  }

  ~callback_receiver() = default;

  template <typename V = Value, std::enable_if_t<!std::is_same_v<V, void>, bool> = true> void complete(V&& value)
  {
    auto initial_state = receiver_state::initial;
    if (m_state.compare_exchange_strong(initial_state, receiver_state::completed)) {
      std::invoke(std::move(m_complete_handler), std::move(value));
    }
  }

  template <typename T = Value, std::enable_if_t<std::is_same_v<T, void>, bool> = true> void complete()
  {
    auto initial_state = receiver_state::initial;
    if (m_state.compare_exchange_strong(initial_state, receiver_state::completed)) {
      std::invoke(std::move(m_complete_handler));
    }
  }

  void fail(std::exception_ptr e) noexcept
  {
    auto initial_state = receiver_state::initial;
    if (m_state.compare_exchange_strong(initial_state, receiver_state::failed)) {
      std::invoke(std::move(m_error_handler), e);
    }
  }

  void cancel() noexcept
  {
    auto initial_state = receiver_state::initial;
    if (m_state.compare_exchange_strong(initial_state, receiver_state::canceled)) {
      std::invoke(std::move(m_cancel_handler));
    }
  }

  bool is_canceled() const noexcept { return receiver_state::failed != m_state.load(); }

private:
  std::atomic<receiver_state> m_state;
  CompleteHandler m_complete_handler;
  ErrorHandler m_error_handler;
  CancelHandler m_cancel_handler;
};

template <typename CompleteHandler, typename ErrorHandler, typename CancelHandler,
          std::enable_if_t<(invocable<CompleteHandler>::size > 1U), bool> = true>
auto make_callback_receiver(CompleteHandler complete_handler, ErrorHandler error_handler, CancelHandler cancel_handler)
{
  static_assert(invocable<CompleteHandler>::size <= 1U, "CompleteHandler supports one argument or no arguments");
}

template <typename CompleteHandler, typename ErrorHandler, typename CancelHandler,
          std::enable_if_t<invocable<CompleteHandler>::size == 1U, bool> = true>
auto make_callback_receiver(CompleteHandler complete_handler, ErrorHandler error_handler, CancelHandler cancel_handler)
{
  using Value = typename invocable<CompleteHandler>::template arg<0>::type;

  static_assert(std::is_invocable_v<CompleteHandler, Value>, "CompleteHandler must accept Value as argument");

  return callback_receiver<Value, CompleteHandler, ErrorHandler, CancelHandler>{
      std::move(complete_handler), std::move(error_handler), std::move(cancel_handler)};
}

template <typename CompleteHandler, typename ErrorHandler, typename CancelHandler,
          std::enable_if_t<invocable<CompleteHandler>::size == 0U, bool> = true>
auto make_callback_receiver(CompleteHandler complete_handler, ErrorHandler error_handler, CancelHandler cancel_handler)
{
  static_assert(std::is_invocable_v<CompleteHandler>, "CompleteHandler must be invocable");

  return callback_receiver<void, CompleteHandler, ErrorHandler, CancelHandler>{
      std::move(complete_handler), std::move(error_handler), std::move(cancel_handler)};
}

}  // namespace jar::concurrency::details

#endif  // JAR_CONCURRENCY_DETAILS_CALLBACK_RECEIVER_HPP
