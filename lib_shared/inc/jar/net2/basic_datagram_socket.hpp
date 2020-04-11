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
/// \file basic_datagram_socket.hpp
///
#ifndef JAR_NET2_BASIC_DATAGRAM_SOCKET_HPP
#define JAR_NET2_BASIC_DATAGRAM_SOCKET_HPP

#include "jar/net2/basic_socket.hpp"
#include "jar/net2/socket_address.hpp"
#include "jar/net2/socket_family.hpp"

namespace jar::net {

/// \brief An interface for datagram sockets
class basic_datagram_socket : public virtual basic_socket {
public:
  /// \brief Deleted copy constructor
  basic_datagram_socket(const basic_datagram_socket&) = delete;
  /// \brief Deleted copy assignment operator
  basic_datagram_socket& operator=(const basic_datagram_socket&) = delete;
  /// \brief Deleted move constructor
  basic_datagram_socket(basic_datagram_socket&&) = delete;
  /// \brief Deleted move assignment operator
  basic_datagram_socket& operator=(basic_datagram_socket&&) = delete;

  /// \brief Destructor
  virtual ~basic_datagram_socket() noexcept = default;

  /// \brief Receive bytes from the remote peer
  ///
  /// \param[out] buffer          Buffer for the received bytes
  /// \param[in]  length          Buffer length
  /// \param[out] remote_address  Remote peer address
  ///
  /// \return Zero when the peer has performed an orderly shutdown; otherwise number of bytes read
  ///
  /// \throws std::system_error
  /// \throws std::invalid_argument
  virtual std::size_t receive(std::uint8_t* buffer, std::size_t length, socket_address& remote_address) = 0;

  /// \brief Send bytes to the remote peer
  ///
  /// \param[in]  buffer          Buffer of containing the bytes to send
  /// \param[in]  length          Buffer length
  /// \param[in]  remote_address  Remote peer address
  ///
  /// \return Number of bytes send
  ///
  /// \throws std::system_error
  /// \throws std::invalid_argument
  virtual std::size_t send(const std::uint8_t* buffer, std::size_t length, const socket_address& remote_address) = 0;

protected:
  /// \brief Constructor
  basic_datagram_socket() = default;
};

/// \brief Makes a datagram socket
///
/// \param[in]  family      Socket family
///
/// \throws std::system_error
std::unique_ptr<basic_datagram_socket> make_datagram_socket(socket_family family);

}  // namespace jar::net

#endif  // JAR_NET2_BASIC_DATAGRAM_SOCKET_HPP
