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
/// \file stream_server_socket.hpp
///
#ifndef JAR_NET_STREAM_SERVER_SOCKET_HPP
#define JAR_NET_STREAM_SERVER_SOCKET_HPP

#include <functional>

#include "jar/net/basic_stream_socket.hpp"
#include "jar/net/socket_address.hpp"

namespace jar::net {

/// \brief Client stream socket forward declaration
class stream_socket;

/// \brief A class representing a stream server socket
///
/// This class can be used to setup a socket server.
class stream_server_socket : public basic_stream_socket {
public:
  /// \brief Handler type for new stream connections
  using handler_t = std::function<void(stream_socket&&)>;

  /// \brief Constructor
  ///
  /// \param[in]  family          Socket family
  ///
  /// \throws std::system_error
  explicit stream_server_socket(socket_family family);

  /// \brief Deleted copy constructor
  stream_server_socket(const stream_server_socket&) = delete;
  /// \brief Deleted copy assignment operator
  stream_server_socket& operator=(const stream_server_socket&) = delete;
  /// \brief Move constructor
  stream_server_socket(stream_server_socket&&) noexcept = default;
  /// \brief Move assignment operator
  stream_server_socket& operator=(stream_server_socket&&) noexcept = default;

  /// \brief Destructor
  ~stream_server_socket() noexcept = default;

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

  /// \brief Start listening
  ///
  /// Socket must be bound before listen.
  ///
  /// \throws std::system_error
  void listen();

  /// \brief Start accepting new connections
  ///
  /// Socket must listening before accept is called.
  ///
  /// \param[in]  handler         Handler for new connections
  ///
  /// \throws std::system_error
  /// \throws std::invalid_argument
  void accept(handler_t&& handler);

private:
  /// \brief Bind to a local address
  ///
  /// \param[in]  local_address   Local address
  /// \param[in]  address_size    Address size in bytes
  ///
  /// \throws std::system_error
  void bind(const ::sockaddr* local_address, std::size_t address_size);
};

}  // namespace jar::net

#endif  // JAR_NET_STREAM_SERVER_SOCKET_HPP
