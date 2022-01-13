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

#ifndef JAR_COM_BASIC_STREAM_SOCKET_HPP
#define JAR_COM_BASIC_STREAM_SOCKET_HPP

#include <jar/core/contract.hpp>

#include "jar/com/basic_socket.hpp"

namespace jar::com {

/// \brief Base class for all stream socket classes
///
/// \tparam Socket      A type that must implement the system specific concepts used by the basic_stream_socket
/// \tparam Protocol    Socket protocol type (e.g. ipc::basic_protocol)
template <typename Socket, typename Protocol> class basic_stream_socket : public basic_socket<Socket, Protocol> {
  /// \brief Short-hand for base type
  using basic_socket_t = basic_socket<Socket, Protocol>;

public:
  /// \brief Deleted copy constructor
  basic_stream_socket(const basic_stream_socket&) = delete;

  /// \brief Deleted copy assignment operator
  basic_stream_socket& operator=(const basic_stream_socket&) = delete;

  /// \brief Receive bytes from the remote peer
  ///
  /// \param[in]  buffer      Buffer for the received bytes
  /// \param[in]  length      Buffer length
  ///
  /// \return Zero when the peer has performed an orderly shutdown; otherwise
  /// number of bytes read
  ///
  /// \throws std::system_error if operation fails due to a system error
  /// \throws std::invalid_argument if the arguments are invalid
  std::size_t receive(std::uint8_t* buffer, std::size_t length)
  {
    contract::not_null(buffer, "buffer cannot be nullptr");
    contract::not_zero(length, "length cannot be zero");
    return Socket::receive(*this, buffer, length);
  }

  /// \brief Send bytes to the remote peer
  ///
  /// \param[in]  buffer      Buffer of containing the bytes to send
  /// \param[in]  length      Buffer length
  ///
  /// \return Number of bytes send
  ///
  /// \throws std::system_error if operation fails due to a system error
  /// \throws std::invalid_argument if the arguments are invalid
  std::size_t send(std::uint8_t const* buffer, std::size_t length)
  {
    contract::not_null(buffer, "buffer cannot be nullptr");
    contract::not_zero(length, "length cannot be zero");
    return Socket::send(*this, buffer, length);
  }

protected:
  /// \brief Native socket handle type
  using native_type = typename basic_socket_t::native_type;

  /// \brief Constructor
  ///
  /// \throws std::system_error if construction fails due to a system error
  basic_stream_socket()
    : basic_socket_t{socket_type::stream}
  {
  }

  /// \brief Constructor
  ///
  /// \param[in]  handle      Native socket handle
  explicit basic_stream_socket(native_type handle) noexcept
    : basic_socket_t{handle}
  {
  }

  /// \brief Default move constructor
  basic_stream_socket(basic_stream_socket&&) noexcept = default;

  /// \brief Default move assignment operator
  basic_stream_socket& operator=(basic_stream_socket&&) noexcept = default;

  /// \brief Protected destructor
  ///
  /// This class is not a polymorphic base class.
  ~basic_stream_socket() = default;
};

}  // namespace jar::com

#endif  // JAR_COM_BASIC_STREAM_SOCKET_HPP
