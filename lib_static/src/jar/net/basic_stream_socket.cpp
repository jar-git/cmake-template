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
/// \file basic_stream_socket.cpp
///
#include "jar/net/basic_stream_socket.hpp"

#include <sys/socket.h>

#include "jar/util/contract.hpp"

namespace jar::net {

using contract = util::contract<int, -1>;

basic_stream_socket::basic_stream_socket(socket_family family)
  : socket{family, socket_type::stream}
{
}

basic_stream_socket::basic_stream_socket(socket_descriptor descriptor, socket_family family)
  : socket{std::move(descriptor), family, socket_type::stream}
{
}

basic_stream_socket::~basic_stream_socket() = default;

std::size_t basic_stream_socket::receive(std::uint8_t* buffer, std::size_t length)
{
  contract::not_null(buffer, "buffer cannot be nullptr");
  contract::not_zero(length, "length cannot be zero");

  const auto bytes_received{::recv(native(), static_cast<void*>(buffer), length, 0)};

  contract::no_system_error(bytes_received);

  return static_cast<std::size_t>(bytes_received);
}

std::size_t basic_stream_socket::send(const std::uint8_t* buffer, std::size_t length)
{
  contract::not_null(buffer, "buffer cannot be nullptr");
  contract::not_zero(length, "length cannot be zero");

  // Disable SIGPIPE, using exceptions.
  const auto bytes_send{::send(native(), static_cast<const void*>(buffer), length, MSG_NOSIGNAL)};

  contract::no_system_error(bytes_send);

  return static_cast<std::size_t>(bytes_send);
}

}  // namespace jar::net
