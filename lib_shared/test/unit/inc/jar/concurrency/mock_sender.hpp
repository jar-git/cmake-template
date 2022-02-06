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
/// \file mock_sender.hpp
///

#ifndef JAR_CONCURRENCY_MOCK_SENDER_HPP
#define JAR_CONCURRENCY_MOCK_SENDER_HPP

#include <gmock/gmock.h>

#include <utility>

#include <jar/concurrency/utilities.hpp>

namespace jar::concurrency::utilities::test {

template<typename Value>
class mock_receiver {
public:
  MOCK_METHOD(void, complete, (Value));
  MOCK_METHOD(void, fail, ());
  MOCK_METHOD(void, cancel, ());

  class delegate {
  public:
    delegate(mock_receiver* receiver)
      : m_instance{receiver}
    {
    }

    void complete(Value value) { m_instance->complete(std::move(value)); }
    void fail() { m_instance->fail(); }
    void cancel() { m_instance->cancel(); }

  private:
    mock_receiver* m_instance;
  };

  auto make_delegate() { return delegate{this}; }
};

template <typename Receiver> class mock_state {
public:
  mock_state(Receiver&& receiver)
    : m_receiver{std::move(receiver)}
  {
  }

  void start() { m_receiver.complete(); }

  template <typename T = Receiver, std::enable_if_t<has_future<T>::value, bool> = true> auto get_future()
  {
    return m_receiver.get_future();
  }

  private:
  Receiver m_receiver;
};

class mock_sender {
public:
  template <typename Receiver> auto connect(Receiver receiver) { return mock_state<Receiver>{std::move(receiver)}; }
};

template <typename Sender, typename Invocable> auto make_sender_adapter(Sender&& sender, Invocable&& invocable)
{
  return sender_adapter{std::forward<Sender>(sender), std::forward<Invocable>(invocable)};
}

}  // namespace jar::concurrency::utilities::test

#endif  // JAR_CONCURRENCY_MOCK_SENDER_HPP
