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
/// \file basic_socket.hpp
///

#ifndef JAR_NET2_SOCKET_HPP
#define JAR_NET2_SOCKET_HPP

#include "jar/fs2/file_descriptor.hpp"
#include "jar/net2/basic_socket.hpp"

#include "jar/net2/shutdown_mode.hpp"
#include "jar/net2/socket_family.hpp"
#include "jar/net2/socket_type.hpp"

namespace jar::net {

using socket_descriptor = fs::file_descriptor;

/// \brief Basic socket functionality
class socket : public virtual basic_socket {
public:
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

  /// \brief Destructor
  ~socket() override = default;

  void bind(const socket_address& local_address) override;
  void open() override;
  bool is_open() const noexcept override { return m_descriptor.is_open(); }
  void close() override { return m_descriptor.close(); }
  void non_blocking(bool mode) override { return m_descriptor.non_blocking(mode); }
  bool is_non_blocking() const override { return m_descriptor.is_non_blocking(); }
  void shutdown(shutdown_mode mode) override;
  std::size_t send_buffer_size() const override;

protected:
  /// \brief Gets the native socket descriptor
  ///
  /// \return Native descriptor
  [[nodiscard]] int native() const noexcept { return m_descriptor.native(); }

  /// \brief Gets the socket family
  ///
  /// \return Socket family
  [[nodiscard]] socket_family family() const noexcept { return m_family; }

private:
  socket_descriptor m_descriptor;
  socket_family m_family;
  socket_type m_type;
};

}  // namespace jar::net

#endif  // JAR_NET2_SOCKET_HPP
