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

#ifndef JAR_COM_BASIC_SOCKET_HPP
#define JAR_COM_BASIC_SOCKET_HPP

#include <chrono>

#include <jar/system/basic_handle.hpp>

#include "shutdown_mode.hpp"
#include "socket_family.hpp"
#include "socket_protocol.hpp"
#include "socket_type.hpp"

namespace jar::com {

/// \brief A RAII class that represents a socket
///
/// This class is an immutable handle to a socket with automatic lifetime management. Only exception to being immutable
/// is the support for move semantics that shall invalidate the socket instance when moved from.
///
/// \tparam Socket      A type that must implement the system specific concepts used by the basic_socket
/// \tparam Protocol    Socket protocol type (e.g. ipc::basic_protocol)
template <typename Socket, typename Protocol>
class basic_socket : public system::basic_handle<Socket>, public Protocol {
  /// \brief Short-hand for base type
  using handle_type = system::basic_handle<Socket>;

public:
  /// \brief Deleted copy constructor
  basic_socket(const basic_socket&) = delete;

  /// \brief Deleted copy assignment operator
  basic_socket& operator=(const basic_socket&) = delete;

  /// \brief Sets the non-blocking mode of the socket
  ///
  /// \param[in]  mode        Non-blocking mode
  ///
  /// \throws std::system_error if operation fails due to a system error
  void non_blocking(bool mode) { Socket::non_blocking(*this, mode); }

  /// \brief Gets the non-blocking mode of the socket
  ///
  /// \param[in]  mode        Non-blocking mode
  ///
  /// \throws std::system_error if operation fails due to a system error
  [[nodiscard]] bool is_non_blocking() const { return Socket::is_non_blocking(*this); }

  /// \brief Shutdown socket
  ///
  /// \param[in]  mode        Shutdown mode (default: both)
  ///
  /// \throws std::system_error if operation fails due to a system error
  void shutdown(shutdown_mode mode = shutdown_mode::both) { Socket::shutdown(*this, mode); }

  /// \brief Gets the socket's send buffer size
  ///
  /// \return Send buffer size
  ///
  /// \throws std::system_error if operation fails due to a system error
  [[nodiscard]] std::size_t get_send_buffer_size() const { return Socket::get_send_buffer_size(*this); }

  /// \brief Sets socket timeouts
  ///
  /// \param[in]  timeout     Socket timeouts
  ///
  /// \tparam R   See https://en.cppreference.com/w/cpp/chrono/duration
  /// \tparam P   See https://en.cppreference.com/w/cpp/chrono/duration
  template <class R, class P> void set_timeout(std::chrono::duration<R, P> timeout)
  {
    Socket::set_send_timeout(*this, std::chrono::duration_cast<std::chrono::microseconds>(timeout));
    Socket::set_receive_timeout(*this, std::chrono::duration_cast<std::chrono::microseconds>(timeout));
  }

  /// \brief Sets socket send timeout
  ///
  /// \param[in]  timeout     Socket timeout
  ///
  /// \tparam R   See https://en.cppreference.com/w/cpp/chrono/duration
  /// \tparam P   See https://en.cppreference.com/w/cpp/chrono/duration
  template <class R, class P> void set_send_timeout(std::chrono::duration<R, P> timeout)
  {
    Socket::set_send_timeout(*this, std::chrono::duration_cast<std::chrono::microseconds>(timeout));
  }

  /// \brief Sets socket receive timeout
  ///
  /// \param[in]  timeout     Socket timeout
  ///
  /// \tparam R   See https://en.cppreference.com/w/cpp/chrono/duration
  /// \tparam P   See https://en.cppreference.com/w/cpp/chrono/duration
  template <class R, class P> void set_receive_timeout(std::chrono::duration<R, P> timeout)
  {
    Socket::set_receive_timeout(*this, std::chrono::duration_cast<std::chrono::microseconds>(timeout));
  }

protected:
  /// \brief Native socket handle type
  using native_type = typename handle_type::native_type;

  /// \brief Constructor
  ///
  /// \param[in]  type    Socket type
  ///
  /// \throws std::system_error if construction fails due to a system error
  explicit basic_socket(socket_type type)
    : handle_type{Protocol::family(), type, Protocol::protocol()}
  {
  }

  /// \brief Constructor
  ///
  /// \param[in]      handle      Native handle
  constexpr explicit basic_socket(native_type handle) noexcept
    : handle_type{handle}
  {
  }

  /// \brief Default move constructor
  basic_socket(basic_socket&&) noexcept = default;

  /// \brief Default move assignment operator
  basic_socket& operator=(basic_socket&&) noexcept = default;

  /// \brief Protected destructor
  ///
  /// This class is not a polymorphic base class.
  ~basic_socket() = default;
};

}  // namespace jar::com

#endif  // JAR_COM_BASIC_SOCKET_HPP
