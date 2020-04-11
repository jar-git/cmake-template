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
/// \file socket_type.hpp
///

#ifndef JAR_NET2_SOCKET_TYPE_HPP
#define JAR_NET2_SOCKET_TYPE_HPP

#include <cstdlib>
#include <type_traits>

namespace jar::net {

/// \brief An enumeration representing the type of the socket
enum class socket_type : int {
  stream = 1,   ///< Stream socket
  datagram = 2  ///< Datagram socket
};

/// \brief Converts socket type to an integral type
///
/// \param[in]  type    Socket type
///
/// \return Socket type value as integral value
[[nodiscard]] inline auto to_integral(socket_type mode) noexcept
{
  return static_cast<std::underlying_type_t<socket_type>>(mode);
}

/// \brief Converts type to a string literal
///
/// \param[in]  type    Socket type
///
/// \return Socket type as string literal
[[nodiscard]] inline auto to_literal(socket_type type) noexcept
{
  switch (type) {
  case socket_type::stream:
    return "stream";
  case socket_type::datagram:
    return "datagram";
    // LCOV_EXCL_START
  default:
    std::abort();
    // LCOV_EXCL_STOP
  }
}

}  // namespace jar::net

#endif  // JAR_NET2_SOCKET_TYPE_HPP
