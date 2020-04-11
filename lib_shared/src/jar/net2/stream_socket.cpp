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
/// \file basicstreamsocket.cpp
///

#include "stream_socket.hpp"

#include <sys/socket.h>

#include "jar/util2/contract.hpp"

namespace jar::net {

using contract = jar::util::contract<int, -1>;

std::unique_ptr<basic_stream_socket> make_stream_socket(socket_family family)
{
  return std::make_unique<stream_socket>(family);
}

stream_socket::stream_socket(socket_family family)
  : socket{family, socket_type::stream}
{
}

stream_socket::stream_socket(socket_descriptor descriptor, socket_family family)
  : socket{std::move(descriptor), family, socket_type::stream}
{
}

void stream_socket::listen() { contract::no_system_error(::listen(native(), SOMAXCONN)); }

void stream_socket::accept(basic_stream_socket::handler_t&& handler)
{
  contract::not_null(handler, "handler cannot be nullptr");

  const auto socket_handle{::accept(native(), nullptr, nullptr)};

  contract::no_system_error(socket_handle);

  handler(std::make_unique<stream_socket>(socket_descriptor{socket_handle}, family()));
}

void stream_socket::connect(const socket_address& remote_address)
{
  ::socklen_t length{static_cast<::socklen_t>(remote_address.size())};
  contract::no_system_error(::connect(native(), remote_address.address(), length));
}

basic_stream_socket::credentials stream_socket::get_credentials()
{
  ::ucred credentials{};
  ::socklen_t length{sizeof(::ucred)};

  contract::no_system_error(::getsockopt(native(), SOL_SOCKET, SO_PEERCRED, &credentials, &length));

  return stream_socket::credentials{credentials.pid, credentials.uid, credentials.gid};
}

std::size_t stream_socket::receive(std::uint8_t* buffer, std::size_t length)
{
  contract::not_null(buffer, "buffer cannot be nullptr");
  contract::not_zero(length, "length cannot be zero");

  const auto bytes_received{::recv(native(), static_cast<void*>(buffer), length, 0)};

  contract::no_system_error(bytes_received);

  return static_cast<std::size_t>(bytes_received);
}

std::size_t stream_socket::send(const std::uint8_t* buffer, std::size_t length)
{
  contract::not_null(buffer, "buffer cannot be nullptr");
  contract::not_zero(length, "length cannot be zero");

  // Disable SIGPIPE, using exceptions.
  const auto bytes_send{::send(native(), static_cast<const void*>(buffer), length, MSG_NOSIGNAL)};

  contract::no_system_error(bytes_send);

  return static_cast<std::size_t>(bytes_send);
}

}  // namespace jar::net
