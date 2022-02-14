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
/// \file type_traits.hpp
///

#ifndef JAR_CONCURRENCY_SCHEDULER_TYPE_TRAITS_HPP
#define JAR_CONCURRENCY_SCHEDULER_TYPE_TRAITS_HPP

#include <functional>
#include <tuple>
#include <type_traits>

namespace jar::concurrency {
namespace details {

template <typename> struct invocable_impl;

template <typename R, typename... Args> struct invocable_impl<R (*)(Args...)> {
  using return_type = R;

  using args_as_tuple = std::tuple<std::decay_t<Args>...>;

  inline static constexpr auto size = sizeof...(Args);

  template <std::size_t Index> struct arg {
    using type = std::decay_t<typename std::tuple_element<Index, args_as_tuple>::type>;
  };
};

template <typename R, typename F, typename... Args> struct invocable_impl<R (F::*)(Args...)> {
  using return_type = R;

  using args_as_tuple = std::tuple<std::decay_t<Args>...>;

  inline static constexpr auto size = sizeof...(Args);

  template <std::size_t Index> struct arg {
    using type = std::decay_t<typename std::tuple_element<Index, args_as_tuple>::type>;
  };
};

template <typename R, typename F, typename... Args> struct invocable_impl<R (F::*)(Args...) const> {
  using return_type = R;

  using args_as_tuple = std::tuple<std::decay_t<Args>...>;

  inline static constexpr auto size = sizeof...(Args);

  template <std::size_t Index> struct arg {
    using type = std::decay_t<typename std::tuple_element<Index, args_as_tuple>::type>;
  };
};
}  // namespace details

template <typename Scheduler, typename = void> struct is_output_scheduler : std::false_type {
};

template <typename Scheduler>
struct is_output_scheduler<
    Scheduler, std::void_t<decltype(std::declval<Scheduler>().schedule(std::declval<std::function<void(void)>>()))>>
  : std::true_type {
};

template <typename Scheduler, typename = void> struct is_input_scheduler : std::false_type {
};

template <typename Scheduler>
struct is_input_scheduler<Scheduler, std::void_t<decltype(std::declval<Scheduler>().scheduled())>> : std::true_type {
};

template <typename Scheduler, typename = void> struct has_scheduler_adapter : std::false_type {
};

template <typename Scheduler>
struct has_scheduler_adapter<Scheduler, std::void_t<decltype(std::declval<Scheduler>().get_adapter())>>
  : std::true_type {
};

template <typename T, typename = void> struct has_future : std::false_type {
};

template <typename T> struct has_future<T, std::void_t<decltype(std::declval<T>().get_future())>> : std::true_type {
};

template <typename Invocable, typename = void> struct invocable : details::invocable_impl<Invocable> {
};

template <typename Invocable>
struct invocable<Invocable, std::void_t<decltype(&Invocable::operator())>>
  : details::invocable_impl<decltype(&Invocable::operator())> {
};

}  // namespace jar::concurrency

#endif  // JAR_CONCURRENCY_SCHEDULER_TYPE_TRAITS_HPP
