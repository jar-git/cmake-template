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
/// \file datagram_socket.hpp
///

#ifndef JAR_NET2_DATAGRAM_SOCKET_HPP
#define JAR_NET2_DATAGRAM_SOCKET_HPP

#include "jar/net2/basic_datagram_socket.hpp"
#include "jar/net2/socket.hpp"

namespace jar::net {

/// \brief Datagram socket implementation
class datagram_socket : public socket, public basic_datagram_socket {
public:
  /// \brief Constructor
  ///
  /// \param[in]  family      Socket family
  ///
  /// \throws std::system_error
  explicit datagram_socket(socket_family family);

  std::size_t receive(std::uint8_t* buffer, std::size_t length, socket_address& remote_address) override;
  std::size_t send(const std::uint8_t* buffer, std::size_t length, const socket_address& remote_address) override;
};

}  // namespace jar::net

#endif  // JAR_NET2_DATAGRAM_SOCKET_HPP
