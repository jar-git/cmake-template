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
/// \file basic_stream_socket.hpp
///
#ifndef JAR_NET_BASIC_STREAM_SOCKET_HPP
#define JAR_NET_BASIC_STREAM_SOCKET_HPP

#include "jar/net/socket.hpp"

namespace jar::net {

/// \brief An abstract class representing a stream socket
///
/// Provides common operations for stream sockets
class basic_stream_socket : public socket {
public:
  /// \brief Receive bytes from the remote peer
  ///
  /// \param[in]  buffer      Buffer for the received bytes
  /// \param[in]  length      Buffer length
  ///
  /// \return Zero when the peer has performed an orderly shutdown; otherwise number of bytes read
  ///
  /// \throws std::system_error
  /// \throws std::invalid_argument
  std::size_t receive(std::uint8_t* buffer, std::size_t length);

  /// \brief Send bytes to the remote peer
  ///
  /// \param[in]  buffer      Buffer of containing the bytes to send
  /// \param[in]  length      Buffer length
  ///
  /// \return Number of bytes send
  ///
  /// \throws std::system_error
  /// \throws std::invalid_argument
  std::size_t send(const std::uint8_t* buffer, std::size_t length);

protected:
  /// \brief Constructor
  ///
  /// \param[in]  family      Socket family
  ///
  /// \throws std::system_error
  explicit basic_stream_socket(socket_family family);

  /// \brief Constructor
  ///
  /// \param[in]  descriptor  Socket descriptor
  /// \param[in]  family      Socket family
  basic_stream_socket(socket_descriptor descriptor, socket_family family);

  /// \brief Deleted copy constructor
  basic_stream_socket(const basic_stream_socket&) = delete;
  /// \brief Deleted copy assignment operator
  basic_stream_socket& operator=(const basic_stream_socket&) = delete;
  /// \brief Move constructor
  basic_stream_socket(basic_stream_socket&&) noexcept = default;
  /// \brief Move assignment operator
  basic_stream_socket& operator=(basic_stream_socket&&) noexcept = default;

  /// \brief Protected destructor
  ///
  /// This class is not a polymorphic base class.
  ~basic_stream_socket();
};

}  // namespace jar::net

#endif  // JAR_NET_BASIC_STREAM_SOCKET_HPP
