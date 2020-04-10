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

#ifndef JAR_NET_SOCKET_HPP
#define JAR_NET_SOCKET_HPP

#include "jar/fs/file_descriptor.hpp"

#include "../../../../lib_header/inc/jar/net/shutdown_mode.hpp"
#include "../../../../lib_header/inc/jar/net/socket_family.hpp"
#include "../../../../lib_header/inc/jar/net/socket_type.hpp"

namespace jar::net {

/// \brief Socket descriptor type
using socket_descriptor = fs::file_descriptor;

/// \brief Base class for all socket types
class socket {
public:
  /// \brief Open the socket
  ///
  /// Should only be called if explicitly closed.
  ///
  /// \throws std::system_error
  /// \throws std::domain_error
  void open();

  /// \brief Checks if the socket descriptor is open
  ///
  /// \return True if open; otherwise false
  [[nodiscard]] constexpr bool is_open() const noexcept { return m_descriptor.is_open(); }

  /// \brief Close the socket
  ///
  /// \throws std::system_error
  void close() { m_descriptor.close(); }

  /// \brief Sets the non-blocking mode of the socket
  ///
  /// \param[in]  mode        Non-blocking mode
  ///
  /// \throws std::system_error
  void non_blocking(bool mode) { m_descriptor.non_blocking(mode); }

  /// \brief Gets the non-blocking mode of the socket
  ///
  /// \param[in]  mode        Non-blocking mode
  ///
  /// \throws std::system_error
  [[nodiscard]] bool is_non_blocking() const { return m_descriptor.is_non_blocking(); }

  /// \brief Shutdown socket
  ///
  /// \param[in]  mode        Shutdown mode (default: both)
  ///
  /// \throws std::system_error
  void shutdown(shutdown_mode mode = shutdown_mode::both);

  /// \brief Gets the socket's send buffer size
  ///
  /// \return Send buffer size
  ///
  /// \throws std::system_error
  [[nodiscard]] std::size_t send_buffer_size() const;

protected:
  /// \brief Constructor
  ///
  /// \param[in]  family      Socket family
  /// \param[in]  type        Socket type
  ///
  /// \throws std::system_error
  socket(socket_family family, socket_type type);

  /// \brief Constructor
  ///
  /// \param[in]  descriptor  Socket descriptor
  /// \param[in]  family      Socket family
  /// \param[in]  type        Socket type
  socket(socket_descriptor descriptor, socket_family family, socket_type type);

  /// \brief Deleted copy constructor
  socket(const socket&) = delete;

  /// \brief Deleted copy assignment operator
  socket& operator=(const socket&) = delete;

  /// \brief Move constructor
  socket(socket&&) noexcept = default;

  /// \brief Move assignment operator
  socket& operator=(socket&&) noexcept = default;

  /// \brief Protected destructor
  ///
  /// This class is not a polymorphic base class.
  ~socket() = default;

  /// \brief Gets the native socket descriptor
  ///
  /// \return Native descriptor
  [[nodiscard]] constexpr int native() const noexcept { return m_descriptor.native(); }

  /// \brief Gets the socket family
  ///
  /// \return Socket family
  [[nodiscard]] constexpr socket_family family() const noexcept { return m_family; }

private:
  socket_descriptor m_descriptor;
  socket_family m_family;
  socket_type m_type;
};

}  // namespace jar::net

#endif  // JAR_NET_SOCKET_HPP
