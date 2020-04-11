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
/// \file file_descriptor.cpp
///
#include "jar/fs2/file_descriptor.hpp"

#include <fcntl.h>
#include <unistd.h>

#include "jar/util2/contract.hpp"

namespace jar::fs {

using contract = jar::util::contract<int, -1>;

file_descriptor::~file_descriptor()
{
  try {
    if (is_open()) {
      close();
    }
  } catch (const std::exception& e) {
    std::abort();
  }
}

void file_descriptor::close()
{
  auto return_value{::close(m_value)};
  m_value = s_invalid_value;
  contract::no_system_error(return_value);
}

void file_descriptor::non_blocking(bool mode)
{
  const auto flags{get_descriptor_flags()};
  int new_flags{mode ? (flags | O_NONBLOCK) : (flags & ~O_NONBLOCK)};
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
  contract::no_system_error(::fcntl(m_value, F_SETFL, new_flags));
}

[[nodiscard]] bool file_descriptor::is_non_blocking() const
{
  const auto flags{get_descriptor_flags()};
  return ((flags & O_NONBLOCK) == O_NONBLOCK);
}

[[nodiscard]] int file_descriptor::get_descriptor_flags() const
{
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
  const auto flags{::fcntl(m_value, F_GETFL, nullptr)};
  contract::no_system_error(flags);
  return flags;
}

}  // namespace jar::fs
