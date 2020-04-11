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
/// \file basicstreamsocket.hpp
///

#ifndef JAR_NET2_STREAM_SOCKET_HPP
#define JAR_NET2_STREAM_SOCKET_HPP

#include "jar/net2/basic_stream_socket.hpp"
#include "jar/net2/socket.hpp"

namespace jar::net {

class stream_socket : public socket, public basic_stream_socket {
public:
  /// \brief Constructor
  ///
  /// \param[in]  family      Socket family
  ///
  /// \throws std::system_error
  explicit stream_socket(socket_family family);

  /// \brief Constructor
  ///
  /// \param[in]  descriptor      Socket descriptor
  /// \param[in]  family          Socket family
  stream_socket(socket_descriptor descriptor, socket_family family);

  void listen() override;
  void accept(handler_t&& handler) override;
  void connect(const socket_address& remote_address) override;
  credentials get_credentials() override;
  std::size_t receive(std::uint8_t* buffer, std::size_t length) override;
  std::size_t send(const std::uint8_t* buffer, std::size_t length) override;
};

}  // namespace jar::net

#endif  // JAR_NET2_STREAM_SOCKET_HPP
