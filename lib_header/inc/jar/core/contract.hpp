/// Copyright 2020 Jani Arola, All rights reserved.
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
/// \file contract.hpp
///

#ifndef JAR_CORE_CONTRACT_HPP
#define JAR_CORE_CONTRACT_HPP

#include <system_error>

#include "type_traits.hpp"

/// \brief Namespace containing pre and post condition utilities
///
/// \ingroup core
///
namespace jar::contract {

/// \brief Converts system error to a std::system_error exception and throws it
///
/// \param[in]  return_value    Return value from a system API
///
/// \throws std::system_error if return_value matches system error value
///
/// \tparam R   Return type of the system API
/// \tparam E   Error value (default: -1)
template <typename R, R E = -1, typename... I> constexpr void no_system_error(R return_value)
{
  return_value == E ? throw std::system_error{errno, std::system_category()} : 0;
}

/// \brief Converts system error to a std::system_error exception and throws it
///
/// \param[in]  return_value    Return value from a system API
/// \param[in]  ignored         Ignored system errors (optional)
///
/// \throws std::system_error if return_value matches system error value
///
/// \tparam R   Return type of the system API
/// \tparam I   Ignored errno values
/// \tparam E   Error value (default: -1)
template <typename R, R E = -1, typename... I> constexpr void no_system_error_other_than(R return_value, I... ignored)
{
  bool errno_is_not_any_of = ((errno != ignored) && ...);
  (return_value == E && errno_is_not_any_of) ? throw std::system_error{errno, std::system_category()} : 0;
}

/// \brief Check if the return value from a system API is error
///
/// \param[in]  return_value    Return value
///
/// \return True if return value equals system error; otherwise false.
///
/// \tparam R   Return type of the system API
/// \tparam E   Error value (default: -1)
template <typename R, R E = -1> constexpr bool is_system_error(R return_value) noexcept { return return_value == E; }

/// \brief Converts a null value to a std::invalid_argument exception and throws it
///
/// \param[in]  nullable        A nullable value to check
/// \param[in]  message         Exception message
///
/// \throws std::invalid_argument if value is nullptr
///
/// \tparam T   Nullable type
template <typename T, std::enable_if_t<core::is_nullable_v<typename std::decay_t<T>>, bool> = true>
constexpr void not_null(T&& nullable, char const* const message)
{
  nullable == nullptr ? throw std::invalid_argument{message} : 0;
}

/// \brief Converts zero value to a std::invalid_argument exception and throws it
///
/// \param[in]  value           Value to check
/// \param[in]  message         Exception message
///
/// \throws std::invalid_argument if value is zero
///
/// \tparam T   Value type
template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
constexpr void not_zero(T value, char const* const message)
{
  value == T{0} ? throw std::invalid_argument{message} : 0;
}

/// \brief Converts a value that is greater than max to a std::invalid_argument exception and throws it
///
/// \param[in]  value           Value
/// \param[in]  max             Maximum value
/// \param[in]  message         Exception message
///
/// \throws std::invalid_argument if value is greater than max
///
/// \tparam T   Value type
template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
static constexpr void not_greater(T value, T max, char const* const message)
{
  value > max ? throw std::invalid_argument{message} : 0;
}

/// \brief Converts a value that is less than min to a std::invalid_argument exception and throws it
///
/// \param[in]  value           Value
/// \param[in]  min             Minimum value
/// \param[in]  message         Exception message
///
/// \throws std::invalid_argument if value is less than min
///
/// \tparam T   Value type
template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
static constexpr void not_less(T value, T min, char const* const message)
{
  value < min ? throw std::invalid_argument{message} : 0;
}

}  // namespace jar::contract

#endif  // JAR_CORE_CONTRACT_HPP
