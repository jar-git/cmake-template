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
/// \file socket.cpp
///
#include "jar/net/socket.hpp"

#include <sys/socket.h>

#include "jar/util/contract.hpp"

namespace jar::net {

using contract = jar::util::contract<int, -1>;

socket_descriptor make_descriptor(socket_family family, socket_type type)
{
  // Default protocol is used when a single protocol exists for the socket type. If multiple protocols exist then it
  // must be defined. See: /etc/protocols, https://www.iana.org/assignments/protocol-numbers/protocol-numbers.xhtml
  static constexpr const int default_protocol{0};

  auto return_value{::socket(to_integral(family), to_integral(type), default_protocol)};
  contract::no_system_error(return_value);

  return socket_descriptor{return_value};
}

socket::socket(socket_family family, socket_type type)
  : m_descriptor{make_descriptor(family, type)}
  , m_family{family}
  , m_type{type}
{
}

socket::socket(socket_descriptor descriptor, socket_family family, socket_type type)
  : m_descriptor{std::move(descriptor)}
  , m_family{family}
  , m_type{type}
{
}

void socket::open()
{
  if (is_open()) {
    throw std::domain_error{"Socket is already open"};
  }

  m_descriptor = make_descriptor(m_family, m_type);
}

void socket::shutdown(shutdown_mode mode)
{
  contract::no_system_error(::shutdown(m_descriptor.native(), to_integral(mode)));
}

std::size_t socket::send_buffer_size() const
{
  std::size_t size{};
  ::socklen_t socklen{sizeof(size)};
  contract::no_system_error(
      ::getsockopt(m_descriptor.native(), SOL_SOCKET, SO_SNDBUF, static_cast<void*>(&size), &socklen));
  return size;
}

}  // namespace jar::net
