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
/// \file socket_family.hpp
///

#ifndef JAR_COM_SOCKET_FAMILY_HPP
#define JAR_COM_SOCKET_FAMILY_HPP

#include <iosfwd>

namespace jar::com {

/// \brief An enumeration representing the family of the socket
enum class socket_family : int {
  ipc = 1,  ///< IPC
  ipv4 = 2, ///< IPv4
  ipv6 = 10 ///< IPv6
};

/// \brief Stream extraction operator for socket family
///
/// \param[in|out]  os      Output stream
/// \param[in]      family  Socket family
///
/// \return Output stream
std::ostream &operator<<(std::ostream &os, socket_family family);

} // namespace jar::com

#endif // JAR_COM_SOCKET_FAMILY_HPP
