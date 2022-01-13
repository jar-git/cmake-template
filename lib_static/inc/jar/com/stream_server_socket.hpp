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

#ifndef JAR_COM_STREAM_SERVER_SOCKET_HPP
#define JAR_COM_STREAM_SERVER_SOCKET_HPP

#include <algorithm>
#include <functional>

#include "jar/com/basic_stream_socket.hpp"
#include "jar/com/stream_socket.hpp"

namespace jar::com {

/// \brief A RAII class that represents a stream server socket
///
/// This class is an immutable handle to a stream server socket with automatic lifetime management. Only exception to
/// being immutable is the support for move semantics that shall invalidate the stream server socket instance when moved
/// from. This class can be used to setup a socket server.
///
/// \tparam Socket      A type that must implement the system specific concepts used by the stream_server_socket
/// \tparam Protocol    Socket protocol type (e.g. ipc::basic_protocol)
template <typename Socket, typename Protocol>
class stream_server_socket : public basic_stream_socket<Socket, Protocol> {
public:
  /// \brief Socket address type
  using address_type = typename Protocol::address_type;

  /// \brief Handler type for new stream connections
  using handler_type = std::function<void(stream_socket<Socket, Protocol>&&)>;

  /// \brief Constructor
  ///
  /// \throws std::system_error if construction fails due to a system error
  stream_server_socket()
    : basic_stream_socket<Socket, Protocol>{}
  {
  }

  /// \brief Deleted copy constructor
  stream_server_socket(const stream_server_socket&) = delete;
  /// \brief Deleted copy assignment operator
  stream_server_socket& operator=(const stream_server_socket&) = delete;
  /// \brief Default move constructor
  stream_server_socket(stream_server_socket&&) noexcept = default;
  /// \brief Default move assignment operator
  stream_server_socket& operator=(stream_server_socket&&) noexcept = default;

  /// \brief Destructor
  ~stream_server_socket() = default;

  /// \brief Returns maximum amount of queued connections in listening mode
  ///
  /// \return Max queue size
  constexpr static std::size_t max_que() noexcept { return Socket::max_listen_que(); }

  /// \brief Bind to a local address
  ///
  /// \param[in]  local_address   Local address
  ///
  /// \throws std::system_error if operation fails due to a system error
  /// \throws std::invalid_argument if the arguments are invalid
  void bind(address_type const& local_address)
  {
    using native_address_type = typename address_type::native_type;
    contract::not_zero(local_address.length(), "address cannot be empty");

    if constexpr (socket_family::ipc == Protocol::family() ) {
      local_address.unlink();
    }
    Socket::bind(*this, static_cast<native_address_type const*>(local_address));
  }

  /// \brief Start listening for connections
  ///
  /// \param[in]  queue_size      Amount of queued connections
  ///
  /// Socket must be bound before listen. If queue_size exceeds maximum value
  /// it will be reduced to maximum value. Defaults to max_que().
  ///
  /// \throws std::system_error
  void listen(std::size_t queue_size = max_que()) { Socket::listen(*this, std::min(queue_size, max_que())); }

  /// \brief Start accepting new connections
  ///
  /// Socket must listening before accept is called.
  ///
  /// \param[in]  handler         Handler for new connections
  ///
  /// \throws std::system_error if operation fails due to a system error
  /// \throws std::invalid_argument if the arguments are invalid
  void accept(handler_type&& handler)
  {
    contract::not_null(handler, "handler cannot be nullptr");
    handler(stream_socket<Socket, Protocol>{Socket::accept(*this)});
  }
};

}  // namespace jar::com

#endif  // JAR_COM_STREAM_SERVER_SOCKET_HPP
