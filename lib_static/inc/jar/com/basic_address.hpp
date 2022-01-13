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
/// \file basic_address.hpp
///

#ifndef JAR_COM_BASIC_ADDRESS_HPP
#define JAR_COM_BASIC_ADDRESS_HPP

#include <memory>
#include <string_view>

namespace jar::com {

/// \brief A class representing address for communications
///
/// \tparam AddressTraits       A type that must implement the concepts for creating and destroying native address
template <typename AddressTraits> class basic_address {
public:
  /// \brief Native type of the address
  using native_type = typename AddressTraits::native_type;

  /// \brief Inequality comparison operator
  ///
  /// \param[in]  lh      Left hand side
  /// \param[in]  rh      Right hand side
  ///
  /// \return True if the values are not equal; otherwise false
  friend constexpr bool operator!=(const basic_address<AddressTraits>& lh,
                                   const basic_address<AddressTraits>& rh) noexcept
  {
    return !(lh == rh);
  }

  /// \brief Equality comparison operator
  ///
  /// \param[in]  lh      Left hand side
  /// \param[in]  rh      Right hand side
  ///
  /// \return True if the values are equal; otherwise false
  friend constexpr bool operator==(const basic_address<AddressTraits>& lh,
                                   const basic_address<AddressTraits>& rh) noexcept
  {
    return AddressTraits::equal(lh.m_address, rh.m_address);
  }

  /// \brief Converts address to a native pointer
  ///
  /// \return Native address ptr
  [[nodiscard]] constexpr explicit operator native_type*() noexcept { return std::addressof(m_address); }

  /// \brief Converts address to a native pointer
  ///
  /// \return Native address ptr
  [[nodiscard]] constexpr explicit operator const native_type*() const noexcept { return std::addressof(m_address); }

  /// \brief Converts address to a string view
  ///
  /// \return String representation of the address
  [[nodiscard]] constexpr explicit operator std::string_view() const noexcept
  {
    return AddressTraits::to_string_view(m_address);
  }

  /// \brief Gets address length
  ///
  /// \return Address length
  [[nodiscard]] constexpr std::size_t length() const noexcept { return AddressTraits::length(m_address); }

  /// \brief Gets max address length
  ///
  /// \return Max address length
  [[nodiscard]] constexpr static std::size_t max_length() noexcept { return AddressTraits::max_length(); }

protected:
  /// \brief Constructs an empty address
  constexpr basic_address() noexcept(std::is_nothrow_default_constructible_v<native_type>)
    : m_address{}
  {
  }

  /// \brief Constructor
  ///
  /// \param[in]      args        Arguments for initializing the address
  ///
  /// \tparam Args                Argument types
  template <typename... Args>
  constexpr explicit basic_address(Args&&... args)
    : m_address{AddressTraits::construct(std::forward<Args>(args)...)}
  {
  }

  /// \brief Default copy constructor
  constexpr basic_address(const basic_address&) noexcept(std::is_nothrow_copy_constructible_v<native_type>) = default;

  /// \brief Default move constructor
  constexpr basic_address(basic_address&&) noexcept(std::is_nothrow_move_assignable_v<native_type>) = default;

  /// \brief Default copy assignment operator
  constexpr basic_address&
  operator=(const basic_address&) noexcept(std::is_nothrow_copy_assignable_v<native_type>) = default;

  /// \brief Default move assignment operator
  constexpr basic_address&
  operator=(basic_address&&) noexcept(std::is_nothrow_move_assignable_v<native_type>) = default;

  /// \brief Destructor
  ~basic_address() { AddressTraits::destroy(m_address); }

private:
  native_type m_address;
};

}  // namespace jar::com

#endif  // JAR_COM_BASIC_ADDRESS_HPP
