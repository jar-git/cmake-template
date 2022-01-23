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
/// \file socket.cpp
///

#include "jar/system/posix/socket.hpp"

#include <algorithm>

#include <sys/socket.h>
#include <sys/un.h>

#include <fcntl.h>
#include <unistd.h>

#include "jar/core/contract.hpp"
#include "jar/core/enum.hpp"

namespace jar::system::posix {

using core::to_integral;

[[nodiscard]] socket::native_type socket::construct(com::socket_family family, com::socket_type type,
                                                    com::socket_protocol protocol)
{
  auto return_value{::socket(to_integral(family), to_integral(type), to_integral(protocol))};
  contract::no_system_error(return_value);
  return return_value;
}

void socket::destroy(socket::native_type handle) noexcept
{
  auto result{::close(handle)};
  // NOLINTNEXTLINE (cppcoreguidelines-pro-bounds-array-to-pointer-decay)
  if (contract::is_system_error(result)) {
    std::terminate();
  }
}

[[nodiscard]] bool socket::is_non_blocking(socket::native_type handle)
{
  const auto flags{get_descriptor_flags(handle)};
  return ((flags & O_NONBLOCK) == O_NONBLOCK);
}

void socket::non_blocking(native_type handle, bool mode)
{
  const auto flags{get_descriptor_flags(handle)};
  int new_flags{mode ? (flags | O_NONBLOCK) : (flags & ~O_NONBLOCK)};
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
  contract::no_system_error(::fcntl(handle, F_SETFL, new_flags));
}

void socket::shutdown(native_type handle, com::shutdown_mode mode)
{
  contract::no_system_error(::shutdown(handle, core::to_integral(mode)));
}

[[nodiscard]] std::size_t socket::get_send_buffer_size(native_type handle)
{
  std::size_t size{};
  ::socklen_t socklen{sizeof(size)};
  contract::no_system_error(::getsockopt(handle, SOL_SOCKET, SO_SNDBUF, static_cast<void*>(&size), &socklen));
  return size;
}

void socket::listen(native_type handle, std::size_t que_size)
{
  contract::no_system_error(::listen(handle, static_cast<int>(que_size)));
}

socket::native_type socket::accept(native_type handle)
{
  const auto socket_handle{::accept(handle, nullptr, nullptr)};
  contract::no_system_error(socket_handle);
  return socket_handle;
}

void socket::bind(native_type handle, ::sockaddr const* const local_address, std::size_t address_size)
{
  ::socklen_t length{static_cast<::socklen_t>(address_size)};
  contract::no_system_error(::bind(handle, local_address, length));
}

void socket::connect(native_type handle, const ::sockaddr* remote_address, std::size_t address_size)
{
  ::socklen_t length{static_cast<::socklen_t>(address_size)};
  contract::no_system_error(::connect(handle, remote_address, length));
}

[[nodiscard]] std::size_t socket::receive(native_type handle, std::uint8_t* buffer, std::size_t length)
{
  const auto bytes_received{::recv(handle, static_cast<void*>(buffer), length, 0)};
  contract::no_system_error(bytes_received);
  return static_cast<std::size_t>(bytes_received);
}

[[nodiscard]] std::size_t socket::send(native_type handle, const std::uint8_t* buffer, std::size_t length)
{
  const auto bytes_send{::send(handle, static_cast<const void*>(buffer), length, MSG_NOSIGNAL)};
  contract::no_system_error(bytes_send);
  return static_cast<std::size_t>(bytes_send);
}

[[nodiscard]] std::size_t socket::send_to(native_type handle, const std::uint8_t* buffer, std::size_t length,
                                          ::sockaddr const* const remote_address, std::size_t address_size)
{
  ::socklen_t address_length{static_cast<::socklen_t>(address_size)};
  const auto bytes_send{::sendto(handle, static_cast<const void*>(buffer), length, 0, remote_address, address_length)};
  contract::no_system_error(bytes_send);
  return static_cast<std::size_t>(bytes_send);
}

[[nodiscard]] std::size_t socket::receive_from(native_type handle, std::uint8_t* buffer, std::size_t length,
                                               ::sockaddr* const remote_address, std::size_t address_size)
{
  ::socklen_t address_length{static_cast<::socklen_t>(address_size)};
  const auto bytes_received{::recvfrom(handle, static_cast<void*>(buffer), length, 0, remote_address, &address_length)};
  contract::no_system_error(bytes_received);
  return static_cast<std::size_t>(bytes_received);
}

void socket::set_send_timeout(native_type handle, std::chrono::microseconds microseconds)
{
  ::timeval time_value{};
  time_value.tv_usec = microseconds.count();

  ::socklen_t socklen{sizeof(timeval)};
  contract::no_system_error(::setsockopt(handle, SOL_SOCKET, SO_SNDTIMEO, static_cast<void*>(&time_value), socklen));
}

void socket::set_receive_timeout(native_type handle, std::chrono::microseconds microseconds)
{
  ::timeval time_value{};
  time_value.tv_usec = microseconds.count();

  ::socklen_t socklen{sizeof(timeval)};
  contract::no_system_error(::setsockopt(handle, SOL_SOCKET, SO_RCVTIMEO, static_cast<void*>(&time_value), socklen));
}

[[nodiscard]] int socket::get_descriptor_flags(native_type handle)
{
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
  const auto flags{::fcntl(handle, F_GETFL, nullptr)};
  contract::no_system_error(flags);
  return flags;
}

}  // namespace jar::system::posix
