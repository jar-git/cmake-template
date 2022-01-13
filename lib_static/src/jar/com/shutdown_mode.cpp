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
/// \file shutdown_mode.cpp
///
#include "jar/com/shutdown_mode.hpp"

#include <ostream>

namespace jar::com {

std::ostream& operator<<(std::ostream& os, shutdown_mode mode)
{
  switch (mode) {
  case shutdown_mode::receive:
    return os << "receive";
  case shutdown_mode::send:
    return os << "send";
  case shutdown_mode::both:
    return os << "both";
    // LCOV_EXCL_START
  default:
    os.setstate(std::ios_base::failbit);
    return os;
    // LCOV_EXCL_STOP
  }
}

}  // namespace jar::com
