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
/// \file stream_server_socket.cpp
///
#include "jar/net/stream_server_socket.hpp"

#include <sys/socket.h>
#include <unistd.h>

#include "jar/net/stream_socket.hpp"

#include "jar/util/contract.hpp"

namespace jar::net {

using contract = jar::util::contract<int, -1>;

stream_server_socket::stream_server_socket(socket_family family)
  : basic_stream_socket{family}
{
}

void stream_server_socket::listen() { contract::no_system_error(::listen(native(), SOMAXCONN)); }

void stream_server_socket::accept(handler_t&& handler)
{
  contract::not_null(handler, "handler cannot be nullptr");

  const auto socket_handle{::accept(native(), nullptr, nullptr)};

  contract::no_system_error(socket_handle);

  handler(stream_socket{socket_descriptor{socket_handle}, family()});
}

void stream_server_socket::bind(const ::sockaddr* local_address, std::size_t address_size)
{
  contract::no_system_error(::unlink(&local_address->sa_data[0]));

  ::socklen_t length{static_cast<::socklen_t>(address_size)};
  contract::no_system_error(::bind(native(), local_address, length));
}

}  // namespace jar::net
