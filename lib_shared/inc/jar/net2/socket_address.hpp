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
/// \file socket_address.hpp
///
#ifndef JAR_NET2_SOCKET_ADDRESS_HPP
#define JAR_NET2_SOCKET_ADDRESS_HPP

#include <memory>
#include <string_view>

struct sockaddr;

namespace jar::net {

/// \brief An interface for socket address
class socket_address {
public:
  /// \brief Deleted copy constructor
  socket_address(const socket_address&) = delete;

  /// \brief Deleted copy assignment operator
  socket_address& operator=(const socket_address&) = delete;

  /// \brief Move constructor
  socket_address(socket_address&&) noexcept = default;

  /// \brief Move assignment operator
  socket_address& operator=(socket_address&&) noexcept = default;

  /// \brief Destructor
  virtual ~socket_address() = default;

  /// \brief Inequality comparison operator
  ///
  /// \param[in]  lh      Left hand side
  /// \param[in]  rh      Right hand side
  ///
  /// \return True if the values are not equal; otherwise false
  friend bool operator!=(const socket_address& lh, const socket_address& rh) noexcept { return !(lh == rh); }

  /// \brief Equality comparison operator
  ///
  /// \param[in]  lh      Left hand side
  /// \param[in]  rh      Right hand side
  ///
  /// \return True if the values are equal; otherwise false
  friend bool operator==(const socket_address& lh, const socket_address& rh) noexcept { return lh.is_equal(rh); }

  /// \brief Returns address size
  ///
  /// \return Address size
  [[nodiscard]] virtual std::size_t size() const noexcept = 0;

  /// \brief Sets new address size
  ///
  /// \param[in]  size        New size
  virtual void size(std::size_t size) = 0;

  /// \brief Gets the native socket address
  ///
  /// \return Native descriptor
  [[nodiscard]] virtual ::sockaddr* address() noexcept = 0;

  /// \brief Gets the native socket address
  ///
  /// \return Native descriptor
  [[nodiscard]] virtual const ::sockaddr* address() const noexcept = 0;

protected:
  /// \brief Equality comparison
  ///
  /// \param[in]  rh          Right hand side
  ///
  /// \return True if equal; otherwise false
  virtual bool is_equal(const socket_address& rh) const noexcept = 0;

  /// \brief Constructor
  socket_address() = default;
};

/// \brief Make a domain address
///
/// \param[in]    path        Socket path
///
/// \return Domain address instance
std::unique_ptr<socket_address> make_domain_address(std::string_view path);

}  // namespace jar::net

#endif  // JAR_NET2_SOCKET_ADDRESS_HPP
