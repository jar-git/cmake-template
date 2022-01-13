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
/// \file socket_protocol.hpp
///

#ifndef JAR_COM_SOCKET_PROTOCOL_HPP
#define JAR_COM_SOCKET_PROTOCOL_HPP

#include <iosfwd>

namespace jar::com {

/// \brief An enumeration representing the protocol of the socket
enum class socket_protocol : int {
  unspecified = 0, ///< Default protocol that is appropriate for the socket type
};

/// \brief Stream extraction operator for socket protocol
///
/// \param[in|out]  os          Output stream
/// \param[in]      protocol    Socket protocol
///
/// \return Output stream
std::ostream &operator<<(std::ostream &os, socket_protocol protocol);

} // namespace jar::com

#endif // JAR_COM_SOCKET_PROTOCOL_HPP
