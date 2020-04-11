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
#ifndef JAR_NET_DATAGRAM_SOCKET_HPP
#define JAR_NET_DATAGRAM_SOCKET_HPP

#include "jar/net/socket.hpp"
#include "jar/net/socket_address.hpp"

namespace jar::net {

/// \brief A class that represents a datagram socket
class datagram_socket : public socket {
public:
  /// \brief Constructor
  ///
  /// \param[in]  family      Socket family
  ///
  /// \throws std::system_error
  explicit datagram_socket(socket_family family);

  /// \brief Deleted copy constructor
  datagram_socket(const datagram_socket&) = delete;
  /// \brief Deleted copy assignment operator
  datagram_socket& operator=(const datagram_socket&) = delete;
  /// \brief Move constructor
  datagram_socket(datagram_socket&&) noexcept = default;
  /// \brief Move assignment operator
  datagram_socket& operator=(datagram_socket&&) noexcept = default;

  /// \brief Destructor
  ~datagram_socket() noexcept;

  /// \brief Bind to a local address
  ///
  /// \param[in]  local_address   Local address
  ///
  /// \tparam A   Address type
  ///
  /// \throws std::system_error
  /// \throws std::invalid_argument
  template <typename A> void bind(const socket_address<A>& local_address)
  {
    bind(local_address.address(), local_address.size());
  }

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
  template <typename A> std::size_t receive(std::uint8_t* buffer, std::size_t length, socket_address<A>& remote_address)
  {
    std::size_t address_size{remote_address.size()};
    auto bytes{receive(buffer, length, remote_address.address(), address_size)};
    remote_address.size(address_size);
    return bytes;
  }

  /// \brief Send bytes to the remote peer
  ///
  /// \param[in]  buffer          Buffer of containing the bytes to send
  /// \param[in]  length          Buffer length
  /// \param[in]  remote_address  Remote peer address
  ///
  /// \return Number of bytes send
  ///
  /// \tparam A   Address type
  ///
  /// \throws std::system_error
  /// \throws std::invalid_argument
  template <typename A>
  std::size_t send(const std::uint8_t* buffer, std::size_t length, const socket_address<A>& remote_address)
  {
    return send(buffer, length, remote_address.address(), remote_address.size());
  }

private:
  /// \brief Bind to a local address
  ///
  /// \param[in]  local_address   Local address
  /// \param[in]  address_size    Address size in bytes
  ///
  /// \throws std::system_error
  void bind(const ::sockaddr* local_address, std::size_t address_size);

  /// \brief Send bytes to the remote peer
  ///
  /// \param[in]  buffer          Buffer of containing the bytes to send
  /// \param[in]  length          Buffer length
  /// \param[in]  remote_address  Peer address
  /// \param[in]  address_size    Address size in bytes
  ///
  /// \return Number of bytes send
  ///
  /// \throws std::system_error
  /// \throws std::invalid_argument
  [[nodiscard]] std::size_t send(const std::uint8_t* buffer, std::size_t length, const ::sockaddr* remote_address,
                                 std::size_t address_size);

  /// \brief Receive bytes from the remote peer
  ///
  /// \param[out] buffer          Buffer for the received bytes
  /// \param[in]  length          Buffer length
  /// \param[out] remote_address  Remote address
  /// \param[out] address_size    Address size in bytes
  ///
  /// \return Zero when the peer has performed an orderly shutdown; otherwise number of bytes read
  ///
  /// \throws std::system_error
  /// \throws std::invalid_argument
  [[nodiscard]] std::size_t receive(std::uint8_t* buffer, std::size_t length, ::sockaddr* remote_address,
                                    std::size_t& address_size);
};

}  // namespace jar::net

#endif  // JAR_NET_DATAGRAM_SOCKET_HPP
