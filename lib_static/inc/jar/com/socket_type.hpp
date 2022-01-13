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

#ifndef JAR_COM_SOCKET_TYPE_HPP
#define JAR_COM_SOCKET_TYPE_HPP

#include <iosfwd>

namespace jar::com {

/// \brief An enumeration representing the type of the socket
enum class socket_type : int {
  stream = 1,  ///< Stream socket
  datagram = 2 ///< Datagram socket
};

/// \brief Stream extraction operator for socket type
///
/// \param[in|out]  os          Output stream
/// \param[in]      type        Socket type
///
/// \return Output stream
std::ostream &operator<<(std::ostream &os, socket_type type);

} // namespace jar::net

#endif // JAR_COM_SOCKET_TYPE_HPP
