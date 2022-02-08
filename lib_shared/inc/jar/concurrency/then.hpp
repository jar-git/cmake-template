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

#include <jar/concurrency/details/sender_adapter.hpp>

namespace jar::concurrency {

template <typename Sender, typename Invocable> auto then(Sender&& sender, Invocable&& invocable)
{
  return details::sender_adapter{std::forward<Sender>(sender), std::forward<Invocable>(invocable)};
}

}  // namespace jar::concurrency

#endif  // JAR_CONCURRENCY_THEN_HPP
