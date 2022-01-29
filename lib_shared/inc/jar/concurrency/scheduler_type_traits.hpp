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
/// \file scheduler_type_traits.hpp
///

#ifndef JAR_CONCURRENCY_SCHEDULER_TYPE_TRAITS_HPP
#define JAR_CONCURRENCY_SCHEDULER_TYPE_TRAITS_HPP

#include <functional>

namespace jar::concurrency {

template <typename Scheduler, typename = void> struct is_output_scheduler : std::false_type {
};

template <typename Scheduler>
struct is_output_scheduler<Scheduler, std::enable_if_t<std::is_member_function_pointer_v<decltype(&Scheduler::schedule)>>>
  : std::true_type {
};

template <typename Scheduler, typename = void> struct is_input_scheduler : std::false_type {
};

template <typename Scheduler>
struct is_input_scheduler<Scheduler, std::void_t<decltype(std::declval<Scheduler>().scheduled())>>
  : std::true_type {
};

template <typename Scheduler, typename = void> struct has_scheduler_adapter : std::false_type {
};

template <typename Scheduler>
struct has_scheduler_adapter<Scheduler, std::void_t<decltype(std::declval<Scheduler>().get_adapter())>>
  : std::true_type {
};

}  // namespace jar::concurrency

#endif  // JAR_CONCURRENCY_SCHEDULER_TYPE_TRAITS_HPP
