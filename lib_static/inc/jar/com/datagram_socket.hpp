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

#ifndef JAR_COM_DATAGRAM_SOCKET_HPP
#define JAR_COM_DATAGRAM_SOCKET_HPP

#include "jar/core/contract.hpp"

#include "jar/com/basic_socket.hpp"

namespace jar::com {

/// \brief A RAII class that represents a datagram socket
///
/// This class is an immutable handle to a datagram socket with automatic lifetime management. Only exception to being
/// immutable is the support for move semantics that shall invalidate the datagram socket instance when moved from.
///
/// \tparam Socket      A type that must implement the system specific concepts used by the datagram_socket
/// \tparam Protocol    Socket protocol type (e.g. ipc::basic_protocol)
template <typename Socket, typename Protocol> class datagram_socket : public basic_socket<Socket, Protocol> {
  /// \brief Type alias for base type
  using basic_socket_type = basic_socket<Socket, Protocol>;

public:
  /// \brief Socket address type
  using address_type = typename Protocol::address_type;

  /// \brief Constructor
  ///
  /// \throws std::system_error if construction fails due to a system error
  datagram_socket()
    : basic_socket_type{socket_type::datagram}
  {
  }

  /// \brief Deleted copy constructor
  datagram_socket(const datagram_socket&) = delete;
  /// \brief Deleted copy assignment operator
  datagram_socket& operator=(const datagram_socket&) = delete;
  /// \brief Default move constructor
  datagram_socket(datagram_socket&&) noexcept = default;
  /// \brief Default move assignment operator
  datagram_socket& operator=(datagram_socket&&) noexcept = default;

  /// \brief Destructor
  ~datagram_socket() noexcept = default;

  /// \brief Bind to a local address
  ///
  /// \param[in]  local_address   Local address
  ///
  /// \throws std::system_error if operation fails due to a system error
  /// \throws std::invalid_argument if the arguments are invalid
  void bind(address_type const& local_address)
  {
    using native_address_type = typename address_type::native_type;
    contract::not_zero(local_address.length(), "local_address cannot be empty");

    if constexpr (socket_family::ipc == Protocol::family() ) {
      local_address.unlink();
    }
    Socket::bind(*this, static_cast<native_address_type const*>(local_address));
  }

  /// \brief Send bytes to the remote peer
  ///
  /// \param[in]  remote_address  Remote address
  /// \param[in]  buffer          Buffer of containing the bytes to send
  /// \param[in]  length          Buffer length
  ///
  /// \return Number of bytes send
  ///
  /// \throws std::system_error if operation fails due to a system error
  /// \throws std::invalid_argument if the arguments are invalid
  [[nodiscard]] std::size_t send_to(address_type const& remote_address, const std::uint8_t* buffer,
                                    std::size_t length)
  {
    contract::not_zero(remote_address.length(), "remote_address cannot be empty");
    contract::not_null(buffer, "buffer cannot be nullptr");
    contract::not_zero(length, "length cannot be zero");
    return Socket::send_to(*this, static_cast<native_type const*>(remote_address), buffer, length);
  }

  /// \brief Receive bytes from the remote peer
  ///
  /// \param[out] remote_address  Remote address
  /// \param[out] buffer          Buffer for the received bytes
  /// \param[in]  length          Buffer length
  ///
  /// \return Zero when the peer has performed an orderly shutdown; otherwise
  /// number of bytes read
  ///
  /// \throws std::system_error if operation fails due to a system error
  /// \throws std::invalid_argument if the arguments are invalid
  [[nodiscard]] std::size_t receive_from(address_type& remote_address, std::uint8_t* buffer, std::size_t length)
  {
    contract::not_null(buffer, "buffer cannot be nullptr");
    contract::not_zero(length, "length cannot be zero");
    return Socket::receive_from(*this, static_cast<native_type*>(remote_address), buffer, length);
  }

private:
  /// \brief Type alias for native socket address type
  using native_type = typename address_type::native_type;
};

}  // namespace jar::com

#endif  // JAR_COM_DATAGRAM_SOCKET_HPP
