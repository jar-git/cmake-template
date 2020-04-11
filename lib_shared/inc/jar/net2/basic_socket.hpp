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
/// \file socket.hpp
///

#ifndef JAR_NET2_BASIC_SOCKET_HPP
#define JAR_NET2_BASIC_SOCKET_HPP

#include "jar/net2/shutdown_mode.hpp"
#include "jar/net2/socket_address.hpp"

namespace jar::net {

/// \brief A socket interface
class basic_socket {
public:
  /// \brief Deleted copy constructor
  basic_socket(const basic_socket&) = delete;

  /// \brief Deleted copy assignment operator
  basic_socket& operator=(const basic_socket&) = delete;

  /// \brief Deleted move constructor
  basic_socket(basic_socket&&) = delete;

  /// \brief Deleted move assignment operator
  basic_socket& operator=(basic_socket&&) = delete;

  /// \brief Destructor
  virtual ~basic_socket() = default;

  /// \brief Bind to a local address
  ///
  /// \param[in]  local_address   Local address
  ///
  /// \throws std::system_error
  /// \throws std::invalid_argument
  virtual void bind(const socket_address& local_address) = 0;

  /// \brief Open the socket
  ///
  /// Should only be called if explicitly closed.
  ///
  /// \throws std::system_error
  /// \throws std::domain_error
  virtual void open() = 0;

  /// \brief Checks if the socket descriptor is open
  ///
  /// \return True if open; otherwise false
  [[nodiscard]] virtual bool is_open() const noexcept = 0;

  /// \brief Close the socket
  ///
  /// \throws std::system_error
  virtual void close() = 0;

  /// \brief Sets the non-blocking mode of the socket
  ///
  /// \param[in]  mode        Non-blocking mode
  ///
  /// \throws std::system_error
  virtual void non_blocking(bool mode) = 0;

  /// \brief Gets the non-blocking mode of the socket
  ///
  /// \param[in]  mode        Non-blocking mode
  ///
  /// \throws std::system_error
  [[nodiscard]] virtual bool is_non_blocking() const = 0;

  /// \brief Shutdown socket send and receive
  ///
  /// \throws std::system_error
  void shutdown() { shutdown(shutdown_mode::both); }

  /// \brief Shutdown socket
  ///
  /// \param[in]  mode        Shutdown mode (default: both)
  ///
  /// \throws std::system_error
  virtual void shutdown(shutdown_mode mode) = 0;

  /// \brief Gets the socket's send buffer size
  ///
  /// \return Send buffer size
  ///
  /// \throws std::system_error
  [[nodiscard]] virtual std::size_t send_buffer_size() const = 0;

protected:
  /// \brief Constructor
  basic_socket() = default;
};

}  // namespace jar::net

#endif  // JAR_NET2_BASIC_SOCKET_HPP
