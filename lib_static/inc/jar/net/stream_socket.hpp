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
/// \file stream_socket.hpp
///
#ifndef JAR_NET_STREAM_SOCKET_HPP
#define JAR_NET_STREAM_SOCKET_HPP

#include "jar/net/basic_stream_socket.hpp"
#include "jar/net/socket_address.hpp"

namespace jar::net {

/// \brief A class that represents a stream socket
///
/// This class can be used to connect to a stream socket server.
class stream_socket : public basic_stream_socket {
  friend class stream_server_socket;

public:
  /// \brief A class that represents connection credentials
  class credentials {
    using pid_t = int;
    using uid_t = unsigned;
    using gid_t = unsigned;

  public:
    pid_t m_process_id;
    uid_t m_process_uid;
    gid_t m_process_gid;
  };

  /// \brief Constructor
  ///
  /// \param[in]  family      Socket family
  ///
  /// \throws std::system_error
  explicit stream_socket(socket_family family);

  /// \brief Deleted copy constructor
  stream_socket(const stream_socket&) = delete;
  /// \brief Deleted copy assignment operator
  stream_socket& operator=(const stream_socket&) = delete;
  /// \brief Move constructor
  stream_socket(stream_socket&&) noexcept = default;
  /// \brief Move assignment operator
  stream_socket& operator=(stream_socket&&) noexcept = default;

  /// \brief Destructor
  ~stream_socket() = default;

  /// \brief Connect the socket to a server
  ///
  /// \param[in]  remote_address  Remote endpoint address
  ///
  /// \tparam A   Address type
  ///
  /// \throws std::system_error
  /// \throws std::invalid_argument
  template <typename A> void connect(const socket_address<A>& remote_address)
  {
    connect(remote_address.address(), remote_address.size());
  }

  /// \brief Gets credentials of the peer
  ///
  /// \return Process credentials of the peer
  ///
  /// \throws std::system_error
  [[nodiscard]] credentials get_credentials();

private:
  /// \brief Constructor
  ///
  /// \param[in]  descriptor      Socket descriptor
  /// \param[in]  family          Socket family
  stream_socket(socket_descriptor descriptor, socket_family family);

  /// \brief Connect the socket to a server
  ///
  /// \param[in]  remote_address  Remote address
  /// \param[in]  address_size    Remote address size in bytes
  ///
  /// \throws std::system_error
  void connect(const ::sockaddr* remote_address, std::size_t address_size);
};

}  // namespace jar::net

#endif  // JAR_NET_STREAM_SOCKET_HPP
