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

#include <exception>
#include <memory>

#include <jar/concurrency/future.hpp>

namespace jar::concurrency::details {

template <typename Value> class value_receiver {
public:
  value_receiver()
    : m_value{std::make_shared<promise<Value>>()}
  {
  }

  value_receiver(value_receiver const& other)
    : m_value{other.m_value}
  {
  }

  ~value_receiver() = default;

  template <typename V = Value, std::enable_if_t<!std::is_same_v<V, void>, bool> = true> void complete(V&& value)
  {
    m_value->set_value(std::forward<V>(value));
  }

  template <typename V = Value, std::enable_if_t<std::is_same_v<V, void>, bool> = true> void complete()
  {
    m_value->set_value();
  }

  void fail(std::exception_ptr e) noexcept { m_value->set_exception(e); }

  void cancel() noexcept { m_value->cancel(); }

  bool is_canceled() const noexcept { return m_value->is_canceled(); }

  auto get_future() { return m_value->get_future(); }

private:
  std::shared_ptr<promise<Value>> m_value;
};

}  // namespace jar::concurrency::details

#endif  // JAR_CONCURRENCY_DETAILS_VALUE_RECEIVER_HPP
