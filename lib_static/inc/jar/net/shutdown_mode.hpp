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
/// \file shutdown_mode.hpp
///

#ifndef JAR_NET_SHUTDOWN_MODE_HPP
#define JAR_NET_SHUTDOWN_MODE_HPP

#include <cstdlib>
#include <type_traits>

namespace jar::net {

/// \brief An enumeration representing socket shutdown mode
enum class shutdown_mode : int {
  receive,  ///< Shutdown receive
  send,     ///< Shutdown send
  both      ///< Shutdown both
};

/// \brief Converts shutdown mode to an integral type
///
/// \param[in]  mode    Shutdown mode
///
/// \return Shutdown mode as integral value
inline auto to_integral(shutdown_mode mode) noexcept { return static_cast<std::underlying_type_t<shutdown_mode>>(mode); }

/// \brief Converts mode to a string literal
///
/// \param[in]  mode    Shutdown mode
///
/// \return Shutdown mode as string literal
inline auto to_literal(shutdown_mode mode) noexcept
{
  switch (mode) {
  case shutdown_mode::receive:
    return "receive";
  case shutdown_mode::send:
    return "send";
  case shutdown_mode::both:
    return "both";
    // LCOV_EXCL_START
  default:
    std::abort();
    // LCOV_EXCL_STOP
  }
}

}  // namespace jar::net

#endif  // JAR_NET_SHUTDOWN_MODE_HPP
