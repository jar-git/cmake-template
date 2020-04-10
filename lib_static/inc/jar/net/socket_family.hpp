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
/// \file socket_domain.hpp
///

#ifndef JAR_NET_SOCKET_DOMAIN_HPP
#define JAR_NET_SOCKET_DOMAIN_HPP

#include <cstdlib>
#include <type_traits>

namespace jar::net {

/// \brief An enumeration representing the domain of the socket
enum class socket_family : int {
  domain = 1,  ///< Domain
  ipv4 = 2,    ///< IPv4
  ipv6 = 10    ///< IPv6
};

/// \brief Converts socket type to an integral type
///
/// \param[in]  family  Socket type
///
/// \return Socket type value as integral value
inline auto to_integral(socket_family family) noexcept
{
  return static_cast<std::underlying_type_t<socket_family>>(family);
}

/// \brief Converts type to a string literal
///
/// \param[in]  family  Socket type
///
/// \return Socket type as string literal
inline auto to_literal(socket_family family) noexcept
{
  switch (family) {
  case socket_family::domain:
    return "DOMAIN";
  case socket_family::ipv4:
    return "IPv4";
  case socket_family::ipv6:
    return "IPv6";
    // LCOV_EXCL_START
  default:
    std::abort();
    // LCOV_EXCL_STOP
  }
}

}  // namespace jar::net

#endif  // JAR_NET_SOCKET_DOMAIN_HPP
