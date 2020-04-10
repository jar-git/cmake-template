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
/// \file stream_socket.cpp
///
#include "jar/net/stream_socket.hpp"

#include <sys/socket.h>

#include "jar/util/contract.hpp"

namespace jar::net {

using contract = jar::util::contract<int, -1>;

stream_socket::stream_socket(socket_family family)
  : basic_stream_socket{family}
{
}

stream_socket::stream_socket(socket_descriptor descriptor, socket_family family)
  : basic_stream_socket{std::move(descriptor), family}
{
}

void stream_socket::connect(const ::sockaddr* remote_address, std::size_t address_size)
{
  ::socklen_t length{static_cast<::socklen_t>(address_size)};
  contract::no_system_error(::connect(native(), remote_address, length));
}

stream_socket::credentials stream_socket::get_credentials()
{
  ::ucred credentials{};
  ::socklen_t length{sizeof(::ucred)};

  contract::no_system_error(::getsockopt(native(), SOL_SOCKET, SO_PEERCRED, &credentials, &length));

  return stream_socket::credentials{credentials.pid, credentials.uid, credentials.gid};
}

}  // namespace jar::net
