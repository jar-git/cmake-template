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
/// \file utilities.hpp
///

#ifndef JAR_CONCURRENCY_UTILITIES_HPP
#define JAR_CONCURRENCY_UTILITIES_HPP

#include <functional>
#include <type_traits>
#include <utility>
#include <future>
#include <memory>

#include <jar/concurrency/type_traits.hpp>

namespace jar::concurrency::utilities {

 template <typename... Value> class value_receiver {
 public:
   value_receiver()
     : m_value{std::make_shared<std::promise<Value...>>()}
   {
   }

  void complete(Value&&... value)
  {
    m_value->set_value(std::forward<Value>(value)...);
  }

  void fail() noexcept
  {
    m_value->set_exception(std::current_exception());
  }

  void cancel() noexcept { }

  bool is_canceled() noexcept { return false; }

  auto get_future() { return m_value->get_future(); }

 private:
  std::shared_ptr<std::promise<Value...>> m_value;
};

template <typename Receiver, typename Invocable> class receiver_adapter {
public:
  receiver_adapter(Receiver&& receiver, Invocable&& invocable)
    : m_receiver{std::move(receiver)}
    , m_invocable{std::move(invocable)}
  {
  }

  template <typename... Values> void complete(Values&&... values)
  {
    static_assert(std::is_invocable_v<Invocable, Values...>, "Invocable must accept Values as arguments");

    if constexpr (std::is_same_v<std::invoke_result_t<Invocable, Values...>, void>) {
      std::invoke(std::move(m_invocable), std::forward<Values>(values)...);
      m_receiver.complete();
    } else {
      m_receiver.complete(std::invoke(std::move(m_invocable), std::forward<Values>(values)...));
    }
  }

  void fail() noexcept { m_receiver.fail(); }

  void cancel() noexcept { m_receiver.cancel(); }

  bool is_canceled() noexcept { return m_receiver.is_canceled(); }

  template <typename T = Receiver, std::enable_if_t<has_future<T>::value, bool> = true> auto get_future()
  {
    return m_receiver.get_future();
  }

private:
  Receiver m_receiver;
  Invocable m_invocable;
};

template <typename State> class composite_state {
public:
  composite_state(State&& state)
    : m_state{std::forward<State>(state)}
  {
  }

  void start() { m_state.start(); }

  template <typename T = State, std::enable_if_t<has_future<T>::value, bool> = true> auto get_future()
  {
    return m_state.get_future();
  }

private:
  State m_state;
};

template <typename Sender, typename Invocable> class sender_adapter {
public:
  sender_adapter(Sender&& sender, Invocable&& invocable)
    : m_sender{std::forward<Sender>(sender)}
    , m_invocable{std::forward<Invocable>(invocable)}
  {
  }

  template <typename Receiver> auto connect(Receiver&& receiver)
  {
    return composite_state{m_sender.connect(receiver_adapter{std::forward<Receiver>(receiver), std::move(m_invocable)})};
  }

private:
  Sender m_sender;
  Invocable m_invocable;
};

}  // namespace jar::concurrency::utilities

#endif  // JAR_CONCURRENCY_UTILITIES_HPP
