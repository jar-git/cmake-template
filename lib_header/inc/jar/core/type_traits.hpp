/// Copyright 2021 Jani Arola, All rights reserved.
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

#ifndef JAR_CORE_TYPE_TRAITS_HPP
#define JAR_CORE_TYPE_TRAITS_HPP

#include <type_traits>

/// \defgroup core Core
/// \brief Core functionalities and other utilities
///
///  This namespace contains core functionalities and other utilities used by other namespaces.
///
namespace jar::core {

/// \brief A type trait that detects if the given types support equality
///  comparison
template <typename L, typename R, typename = void>
struct has_equal : std::false_type {};

template <typename L, typename R>
struct has_equal<L, R,
                 std::void_t<decltype(std::declval<L>() == std::declval<R>())>>
  : std::true_type {};

/// \brief A variable alias template for has_equal
template <typename L, typename R>
inline constexpr auto has_equal_v = has_equal<L, R>::value;

/// \brief A type trait that detects if the given type is nullable
template <typename T, typename = void> struct is_nullable : has_equal<T, std::nullptr_t> {};

/// \brief A variable alias template for is_nullable
template <typename T>
inline constexpr auto is_nullable_v = is_nullable<T>::value;

} // namespace jar::core

#endif // JAR_CORE_TYPE_TRAITS_HPP
