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
/// \file start.hpp
///

#ifndef JAR_CONCURRENCY_START_HPP
#define JAR_CONCURRENCY_START_HPP

#include <jar/concurrency/details/value_receiver.hpp>

namespace jar::concurrency {

template <typename Sender> void start(Sender&& sender)
{
  using value_type = typename Sender::result_type;

  auto state = sender.connect(details::value_receiver<value_type>{});
  state.start();
}

}  // namespace jar::concurrency

#endif  // JAR_CONCURRENCY_START_HPP
