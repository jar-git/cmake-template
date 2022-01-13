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

#ifndef JAR_COM_STREAM_SOCKET_HPP
#define JAR_COM_STREAM_SOCKET_HPP

#include "jar/com/basic_stream_socket.hpp"

namespace jar::com {

/// \brief stream_server_socket forward declaration
template <typename Socket, typename Protocol> class stream_server_socket;

/// \brief A RAII class that represents a stream socket
///
/// This class is an immutable handle to a stream socket with automatic lifetime management. Only exception to being
/// immutable is the support for move semantics that shall invalidate the stream socket instance when moved from. This
/// class can be used to connect to a stream socket server.
///
/// \tparam Socket      A type that must implement the system specific concepts used by the stream_socket
/// \tparam Protocol    Socket protocol type (e.g. ipc::basic_protocol)
template <typename Socket, typename Protocol> class stream_socket : public basic_stream_socket<Socket, Protocol> {
  /// \brief Friend declaration for stream_server_socket
  friend class stream_server_socket<Socket, Protocol>;

public:
  /// \brief Socket address type
  using address_type = typename Protocol::address_type;

  /// \brief Constructor
  ///
  /// \throws std::system_error if construction fails due to a system error
  stream_socket() = default;

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
  /// \param[in]  remote_address  Remote address
  ///
  /// \throws std::system_error if operation fails due to a system error
  /// \throws std::invalid_argument if the arguments are invalid
  void connect(address_type const& remote_address)
  {
    using native_address_type = typename address_type::native_type;

    contract::not_zero(remote_address.length(), "remote_address cannot be empty");
    Socket::connect(*this, static_cast<native_address_type const*>(remote_address));
  }

protected:
  /// \brief Native socket handle type
  using native_type = typename Socket::native_type;

  /// \brief Constructor
  ///
  /// \param[in]  handle      Native socket handle
  explicit stream_socket(native_type handle) noexcept
    : basic_stream_socket<Socket, Protocol>{handle}
  {
  }
};

}  // namespace jar::com

#endif  // JAR_COM_STREAM_SOCKET_HPP
