/// Copyright 2021 Jani Arola, All rights reserved.
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
/// \file ipc_address.hpp
///

#ifndef JAR_SYSTEM_POSIX_IPC_ADDRESS
#define JAR_SYSTEM_POSIX_IPC_ADDRESS

#include <string_view>

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <jar/core/contract.hpp>

namespace jar::system::posix {

class ipc_address {
public:
  /// \brief Implement native_type concept
  using native_type = ::sockaddr_un;

  /// \brief Implement construction concept
  [[nodiscard]] constexpr static native_type construct(std::string_view socket_path)
  {
    contract::not_greater(socket_path.size(), max_length(), "socket path is too long");
    // Zero initialize address struct to ensure null termination for sun_path
    ::sockaddr_un ipc_address{};
#if __cplusplus > 201703L
    std::copy_n(socket_path.data(), socket_path.size(), &ipc_address.sun_path[0]);
#else
    for (std::size_t i = 0U; i < socket_path.size(); ++i) {
      // NOLINTNEXTLINE (cppcoreguidelines-pro-bounds-constant-array-index) - bounds checked above
      ipc_address.sun_path[i] = socket_path[i];
    }
#endif
    ipc_address.sun_family = AF_LOCAL;
    return ipc_address;
  }

  /// \brief Implement destroy concept
  constexpr static void destroy(native_type address) noexcept { static_cast<void>(address); }

  /// \brief Implement to_string concept
  [[nodiscard]] constexpr static std::string_view to_string_view(native_type address) noexcept
  {
    return std::string_view{address.sun_path};
  }

  /// \brief Implement max_length concept
  [[nodiscard]] constexpr static std::size_t length(native_type address) noexcept
  {
    return to_string_view(address).size();
  }

  /// \brief Implement max_length concept
  [[nodiscard]] constexpr static std::size_t max_length() noexcept { return (sizeof(::sockaddr_un::sun_path) - 1U); }

  /// \brief Implement equal concept
  [[nodiscard]] constexpr static bool equal(native_type const& lh, native_type const& rh) noexcept
  {
    return std::string_view{lh.sun_path} == std::string_view{rh.sun_path};
  }

  /// \brief Implement unlink concept
  static void unlink(std::string_view socket_path)
  {
    contract::not_zero(socket_path.size(), "address cannot be empty");
    contract::no_system_error_other_than(::unlink(socket_path.data()), ENOENT);
  }
};

}  // namespace jar::system::posix

#endif  // JAR_SYSTEM_POSIX_IPC_ADDRESS
