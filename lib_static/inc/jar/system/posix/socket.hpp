/// Copyright 2021 Jani Arola, All rights reserved.
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

#ifndef JAR_SYSTEM_POSIX_SOCKET
#define JAR_SYSTEM_POSIX_SOCKET

#include <chrono>
#include <cstdint>
#include <type_traits>

#include <sys/socket.h>

#include "jar/com/shutdown_mode.hpp"
#include "jar/com/socket_family.hpp"
#include "jar/com/socket_protocol.hpp"
#include "jar/com/socket_type.hpp"

namespace jar::system::posix {

class socket {
public:
  /// \brief Implement native_type concept
  using native_type = decltype(::socket(0, 0, 0));

  /// \brief Implement invalid handle concept
  [[nodiscard]] constexpr static native_type invalid_value() { return native_type{-1}; }

  [[nodiscard]] constexpr static std::size_t max_listen_que()
  {
    static_assert(SOMAXCONN >= 0, "SOMAXCONN cannot be negative");
    return std::size_t{SOMAXCONN};
  }

  /// \brief Implement construction concept
  [[nodiscard]] static native_type construct(com::socket_family family, com::socket_type type,
                                             com::socket_protocol protocol);

  /// \brief Implement destroy concept
  static void destroy(native_type handle) noexcept;

  /// \brief Implement is_non_blocking concept
  [[nodiscard]] static bool is_non_blocking(native_type handle);

  /// \brief Implement non_blocking concept
  static void non_blocking(native_type handle, bool mode);

  /// \brief Implement shutdown concept
  static void shutdown(native_type handle, com::shutdown_mode mode);

  /// \brief Implement send_buffer_size concept
  [[nodiscard]] static std::size_t get_send_buffer_size(native_type handle);

  /// \brief Implement bind concept
  template <typename AddressType> static void bind(native_type handle, AddressType local_address)
  {
    static_assert(std::is_pointer_v<AddressType>, "local_address must be a pointer");
    using native_address_type = std::remove_pointer_t<AddressType>;
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    bind(handle, reinterpret_cast<const ::sockaddr* const>(local_address), sizeof(native_address_type));
  }

  /// \brief Implement bind concept
  template <typename AddressType> static void connect(native_type handle, AddressType remote_address)
  {
    static_assert(std::is_pointer_v<AddressType>, "local_address must be a pointer");
    using native_address_type = std::remove_pointer_t<AddressType>;
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    connect(handle, reinterpret_cast<const ::sockaddr* const>(remote_address), sizeof(native_address_type));
  }

  /// \brief Implement listen concept
  static void listen(native_type handle, std::size_t que_size);

  /// \brief Implement listen concept
  [[nodiscard]] static native_type accept(native_type handle);

  /// \brief Implement receive concept
  [[nodiscard]] static std::size_t receive(native_type handle, std::uint8_t* buffer, std::size_t length);

  /// \brief Implement send concept
  [[nodiscard]] static std::size_t send(native_type handle, const std::uint8_t* buffer, std::size_t length);

  /// \brief Implement send concept
  template <typename AddressType>
  [[nodiscard]] static std::size_t send_to(native_type handle, AddressType remote_address, const std::uint8_t* buffer,
                                           std::size_t length)
  {
    static_assert(std::is_pointer_v<AddressType>, "remote_address must be a pointer");
    using native_address_type = std::remove_pointer_t<AddressType>;
    return send_to(handle, buffer, length, reinterpret_cast<const ::sockaddr* const>(remote_address),
                   sizeof(native_address_type));
  }

  /// \brief Implement receive concept
  template <typename AddressType>
  [[nodiscard]] static std::size_t receive_from(native_type handle, AddressType remote_address, std::uint8_t* buffer,
                                                std::size_t length)
  {
    static_assert(std::is_pointer_v<AddressType>, "remote_address must be a pointer");
    using native_address_type = std::remove_pointer_t<AddressType>;
    return receive_from(handle, buffer, length, reinterpret_cast<::sockaddr* const>(remote_address),
                        sizeof(native_address_type));
  }

  /// \brief Implement send timeout concept
  static void set_send_timeout(native_type handle, std::chrono::microseconds microseconds);

  /// \brief Implement receive timeout concept
  static void set_receive_timeout(native_type handle, std::chrono::microseconds microseconds);

protected:
  /// \brief Get file descriptor flags
  ///
  /// \return File descriptor flags
  [[nodiscard]] static int get_descriptor_flags(native_type handle);

  /// \brief Bind to local address
  ///
  /// \param handle
  /// \param local_address
  /// \param address_size
  static void bind(native_type handle, ::sockaddr const* const local_address, std::size_t address_size);

  /// \brief Connect to a remote address
  ///
  /// \param handle
  /// \param remote_address
  /// \param address_size
  static void connect(native_type handle, ::sockaddr const* const remote_address, std::size_t address_size);

  /// \brief Send bytes to remove address
  ///
  /// \param handle
  /// \param buffer
  /// \param length
  /// \param remote_address
  /// \param address_size
  ///
  /// \return
  [[nodiscard]] std::size_t static send_to(native_type handle, const std::uint8_t* buffer, std::size_t length,
                                           ::sockaddr const* const remote_address, std::size_t address_size);

  /// \brief
  ///
  /// \param handle
  /// \param buffer
  /// \param length
  /// \param address_size
  ///
  /// \return
  [[nodiscard]] static std::size_t receive_from(native_type handle, std::uint8_t* buffer, std::size_t length,
                                                ::sockaddr* const, std::size_t address_size);

private:
  socket() = default;
};

}  // namespace jar::system::posix

#endif  // JAR_SYSTEM_POSIX_SOCKET
