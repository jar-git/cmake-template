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
/// \file datagram_socket.cpp
///
#include "jar/net/datagram_socket.hpp"

#include <sys/socket.h>
#include <unistd.h>

#include "jar/util/contract.hpp"

namespace jar::net {

using contract = jar::util::contract<int, -1>;

datagram_socket::datagram_socket(socket_family family)
  : socket{family, socket_type::datagram}
{
}

datagram_socket::~datagram_socket() noexcept = default;

void datagram_socket::bind(const ::sockaddr* local_address, std::size_t address_size)
{
  contract::no_system_error(::unlink(&local_address->sa_data[0]));

  ::socklen_t length{static_cast<::socklen_t>(address_size)};
  contract::no_system_error(::bind(native(), local_address, length));
}

std::size_t datagram_socket::send(const std::uint8_t* buffer, std::size_t length, const ::sockaddr* remote_address,
                                  std::size_t address_size)
{
  contract::not_null(buffer, "buffer cannot be nullptr");
  contract::not_zero(length, "length cannot be zero");

  ::socklen_t address_length{static_cast<::socklen_t>(address_size)};
  const auto bytes_send{
      ::sendto(native(), static_cast<const void*>(buffer), length, 0, remote_address, address_length)};

  contract::no_system_error(bytes_send);

  return static_cast<std::size_t>(bytes_send);
}

std::size_t datagram_socket::receive(std::uint8_t* buffer, std::size_t length, ::sockaddr* remote_address,
                                     std::size_t& address_size)
{
  contract::not_null(buffer, "buffer cannot be nullptr");
  contract::not_zero(length, "length cannot be zero");

  ::socklen_t address_length{static_cast<::socklen_t>(address_size)};
  const auto bytes_received{
      ::recvfrom(native(), static_cast<void*>(buffer), length, 0, remote_address, &address_length)};

  contract::no_system_error(bytes_received);

  address_size = static_cast<std::size_t>(address_length);
  return static_cast<std::size_t>(bytes_received);
}

}  // namespace jar::net
