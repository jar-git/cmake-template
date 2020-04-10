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

#ifndef JAR_NET_DOMAIN_ADDRESS_HPP
#define JAR_NET_DOMAIN_ADDRESS_HPP

#include <stdexcept>
#include <string_view>

#include <sys/un.h>

#include "jar/net/socket_address.hpp"

namespace jar::net {

/// \brief A class representing a domain socket address
class domain_address : public socket_address<domain_address> {
public:
  /// \brief Constructor
  domain_address();

  /// \brief Constructor
  ///
  /// \param[in]  socket_path Domain socket path
  explicit domain_address(std::string_view socket_path);

  /// \brief Returns address size
  ///
  /// \return Address size
  [[nodiscard]] std::size_t get_size() const noexcept { return m_size; }

  /// \brief Sets new address size
  ///
  /// \param[in]  size        New size
  ///
  /// \return
  void set_size(std::size_t size);

  /// \brief Gets the native socket address
  ///
  /// \return Native descriptor
  [[nodiscard]] ::sockaddr_un* native() noexcept { return &m_address; }

  /// \brief Gets the native socket address
  ///
  /// \return Native descriptor
  [[nodiscard]] const ::sockaddr_un* native() const noexcept { return &m_address; }

  /// \brief Equality comparison
  ///
  /// \param[in]  rh          Right hand side
  ///
  /// \return True if equal; otherwise false
  [[nodiscard]] bool is_equal(const domain_address& rh) const noexcept
  {
    return (m_address.sun_family == rh.m_address.sun_family) &&
        (std::string_view{&m_address.sun_path[0], s_max_size} ==
         std::string_view{&rh.m_address.sun_path[0], s_max_size});
  }

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

#endif  // JAR_NET_DOMAIN_ADDRESS_HPP
