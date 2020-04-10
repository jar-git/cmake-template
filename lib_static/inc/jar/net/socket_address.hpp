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
#ifndef JAR_NET_SOCKET_ADDRESS_HPP
#define JAR_NET_SOCKET_ADDRESS_HPP

struct sockaddr;

namespace jar::net {

/// \brief A class representing socket address
///
/// \tparam T   Socket address type
template <typename T> class socket_address {
public:
  /// \brief Inequality comparison operator
  ///
  /// \param[in]  lh      Left hand side
  /// \param[in]  rh      Right hand side
  ///
  /// \return True if the values are not equal; otherwise false
  friend bool operator!=(const T& lh, const T& rh) noexcept { return !(lh == rh); }

  /// \brief Equality comparison operator
  ///
  /// \param[in]  lh      Left hand side
  /// \param[in]  rh      Right hand side
  ///
  /// \return True if the values are equal; otherwise false
  friend bool operator==(const T& lh, const T& rh) noexcept
  {
    return (lh.implementation().is_equal(rh));
  }

  /// \brief Gets the native socket address
  ///
  /// \return Native descriptor
  [[nodiscard]] ::sockaddr* address() noexcept
  {
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    return reinterpret_cast<::sockaddr*>(implementation().native());
  }

  /// \brief Gets the native socket address
  ///
  /// \return Native descriptor
  [[nodiscard]] const ::sockaddr* address() const noexcept
  {
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    return reinterpret_cast<const ::sockaddr*>(implementation().native());
  }

  /// \brief Returns address size
  ///
  /// \return Address size
  std::size_t size() const noexcept { return implementation().get_size(); }

  /// \brief Sets new address size
  ///
  /// \param[in]  size        New size
  ///
  /// \return
  void size(std::size_t size) { implementation().set_size(size); }

private:
  T& implementation() noexcept { return static_cast<T&>(*this); }
  const T& implementation() const noexcept { return static_cast<const T&>(*this); }
};

}  // namespace jar::net

#endif  // JAR_NET_SOCKET_ADDRESS_HPP
