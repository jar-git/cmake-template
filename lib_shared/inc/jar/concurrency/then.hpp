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
/// \file then.hpp
///

#ifndef JAR_CONCURRENCY_THEN_HPP
#define JAR_CONCURRENCY_THEN_HPP

#include <functional>
#include <type_traits>
#include <utility>

namespace jar::concurrency {
namespace details {

template <typename Receiver, typename Invocable> class then_receiver {
public:
  then_receiver(Receiver&& receiver, Invocable&& invocable)
    : m_receiver{std::forward<Receiver>(receiver)}
    , m_invocable{std::forward<Invocable>(invocable)}
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

  auto get_future() { return m_receiver.get_future(); }

private:
  Receiver m_receiver;
  Invocable m_invocable;
};

template <typename State> class then_state {
public:
  then_state(State&& task)
    : m_state{std::forward<State>(task)}
  {
  }

  void start() { m_state.start(); }

  auto get_future() { return m_state.get_future(); }

private:
  State m_state;
};

template <typename Sender, typename Invocable> class then_sender {
public:
  then_sender(Sender&& sender, Invocable&& invocable)
    : m_sender{std::forward<Sender>(sender)}
    , m_invocable{std::forward<Invocable>(invocable)}
  {
  }

  template <typename Receiver> auto connect(Receiver&& r)
  {
    return then_state{m_sender.connect(then_receiver{std::forward<Receiver>(r), std::forward<Invocable>(m_invocable)})};
  }

private:
  Sender m_sender;
  Invocable m_invocable;
};

}  // namespace details

template <typename Sender, typename Invocable> auto then(Sender&& sender, Invocable&& invocable)
{
  return details::then_sender{std::forward<Sender>(sender), std::forward<Invocable>(invocable)};
}

}  // namespace jar::concurrency

#endif  // JAR_CONCURRENCY_THEN_HPP
