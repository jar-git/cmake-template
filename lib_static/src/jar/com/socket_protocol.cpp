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
/// \file socket_protocol.cpp
///
#include "jar/com/socket_protocol.hpp"

#include <ostream>

namespace jar::com {

std::ostream& operator<<(std::ostream& os, socket_protocol protocol)
{
  switch (protocol) {
  case socket_protocol::unspecified:
    return os << "unspecified";
    // LCOV_EXCL_START
  default:
    os.setstate(std::ios_base::failbit);
    return os;
    // LCOV_EXCL_STOP
  }
}

}  // namespace jar::com
