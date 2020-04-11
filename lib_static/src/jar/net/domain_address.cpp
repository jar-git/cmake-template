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
/// \file domain_address.cpp
///
#include "jar/net/domain_address.hpp"

#include <algorithm>
#include <sys/socket.h>

namespace jar::net {

domain_address::domain_address()
  : m_address{}
  , m_size{s_max_size}
{
}

domain_address::domain_address(std::string_view socket_path)
  : m_address{}
  , m_size{socket_path.size()}
{
  not_too_long(socket_path.size());
  std::copy_n(socket_path.data(), m_size, &m_address.sun_path[0]);
  m_address.sun_family = AF_LOCAL;
  m_size += sizeof(m_address.sun_family);
}

void domain_address::set_size(std::size_t size)
{
  not_too_long(size);
  m_size = size;
}

}  // namespace jar::net
