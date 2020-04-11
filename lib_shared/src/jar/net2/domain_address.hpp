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
/// \file domain_address.hpp
///

#ifndef JAR_NET2_DOMAIN_ADDRESS_HPP
#define JAR_NET2_DOMAIN_ADDRESS_HPP

#include <stdexcept>
#include <string_view>

#include <sys/un.h>

#include "jar/net2/socket_address.hpp"

namespace jar::net {

/// \brief A class representing a domain socket address
class domain_address : public socket_address {
public:
  /// \brief Constructor
  domain_address();

  /// \brief Constructor
  ///
  /// \param[in]  socket_path Domain socket path
  explicit domain_address(std::string_view socket_path);

  std::size_t size() const noexcept override { return m_size; }
  void size(std::size_t size) override;

  ::sockaddr* address() noexcept override
  {
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    return reinterpret_cast<::sockaddr*>(&m_address);
  }

  const ::sockaddr* address() const noexcept override
  {
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    return reinterpret_cast<const ::sockaddr*>(&m_address);
  }

protected:
  bool is_equal(const socket_address& rh) const noexcept override;

private:
  /// \brief Utility function to check that address it not too long
  ///
  /// \param[in]  size        New size
  ///
  /// \throws std::invalid_argument
  static void not_too_long(std::size_t size)
  {
    if (size > s_max_size) {
      throw std::invalid_argument{"address is too long"};
    }
  }

  /// \brief Max size with null termination
  static constexpr const std::size_t s_max_size{sizeof(::sockaddr_un::sun_path) - 1U};

  ::sockaddr_un m_address;
  std::size_t m_size;
};

}  // namespace jar::net

#endif  // JAR_NET2_DOMAIN_ADDRESS_HPP
