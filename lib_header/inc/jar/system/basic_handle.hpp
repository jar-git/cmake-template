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
/// \file basic_handle.hpp
///

#ifndef JAR_SYSTEM_BASIC_HANDLE
#define JAR_SYSTEM_BASIC_HANDLE

#include "jar/core/type_traits.hpp"

/// \defgroup system System
/// \brief Namespace containing abstractions and interfaces for system layer
///
///  This namespace contains abstractions and interfaces for system layer, like sockets, shared memory etc.
///
namespace jar::system {

/// \brief A RAII class that represents a handle to system resource
///
/// This class is an immutable handle to a system resource with automatic lifetime management. Only exception to being
/// immutable is the support for move semantics that shall invalidate the basic_handle instance that was moved from.
///
/// \tparam Resource        A type that must implement the concepts for creating and destroying the native system
///  resource handle
template <typename Resource> class basic_handle {
public:
  /// \brief Gets if basic_handle is valid
  ///
  /// A basic_handle is invalidated after a move operation.
  ///
  /// \return True if valid; otherwise false
  [[nodiscard]] constexpr bool is_valid() const noexcept { return invalid_handle() != m_value; }

  /// \brief Inequality comparison operator for basic handles
  ///
  /// \param[in]      lh          Left hand side
  /// \param[in]      rh          Right hand side
  ///
  /// \return True if the values are not equal; otherwise false
  friend constexpr bool operator!=(const basic_handle<Resource>& lh, const basic_handle<Resource>& rh) noexcept
  {
    return !(lh.m_value == rh.m_value);
  }

  /// \brief Equal comparison operator for basic handles
  ///
  /// \param[in]      lh          Left hand side
  /// \param[in]      rh          Right hand side
  ///
  /// \return True if the values are equal; otherwise false
  friend constexpr bool operator==(const basic_handle<Resource>& lh, const basic_handle<Resource>& rh) noexcept
  {
    static_assert(core::has_equal_v<native_type, native_type>, "native_t must support equality comparison");
    return lh.m_value == rh.m_value;
  }

  /// \brief Deleted copy constructor
  basic_handle(const basic_handle&) = delete;

  /// \brief Deleted copy assignment operator
  basic_handle& operator=(const basic_handle&) = delete;

protected:
  /// \brief Native type of the handle
  using native_type = typename Resource::native_type;

  /// \brief Returns a value that represents an invalid handle
  ///
  /// \return A system specific value for an invalid handle
  [[nodiscard]] constexpr static auto invalid_handle() noexcept { return Resource::invalid_value(); }

  /// \brief Constructor
  ///
  /// \param[in]      args        Arguments to the resource construction
  ///
  /// \tparam Args                Argument types for the resource construction
  ///
  /// \throws std::system_error if the resource construction fails due to a system error
  /// \throws std::invalid_argument if the arguments are invalid
  template <typename... Args>
  constexpr explicit basic_handle(Args&&... args)
    : m_value{Resource::construct(std::forward<Args>(args)...)}
  {
    // Resource concept should throw the exceptions if appropriate.
  }

  /// \brief Constructor for creating a basic_handle from a native value
  ///
  /// \param[in]      handle      Native handle
  constexpr explicit basic_handle(native_type handle) noexcept
    : m_value{handle}
  {
    static_assert(std::is_trivially_copy_constructible_v<native_type> &&
                      std::is_trivially_copy_assignable_v<native_type>,
                  "native_type must be trivially copyable");
  }

  /// \brief Move constructor
  ///
  /// \param[in]      other       Other
  constexpr basic_handle(basic_handle&& other) noexcept
    : m_value{other.m_value}
  {
    other.m_value = invalid_handle();
  }

  /// \brief Move assignment operator
  ///
  /// \param[in]      other       Other
  constexpr basic_handle& operator=(basic_handle&& other) noexcept
  {
    if (this != other) {
      m_value = other.m_value;
      other.m_value = invalid_handle();
    }
  }

  /// \brief Destructor
  ~basic_handle()
  {
    if (is_valid()) {
      Resource::destroy(m_value);
    }
  }

  /// \brief Implicit conversion to native handle type
  ///
  /// \return Native handle
  [[nodiscard]] constexpr operator native_type() const noexcept { return m_value; }

private:
  native_type m_value;
};

}  // namespace jar::system

#endif  // JAR_SYSTEM_BASIC_HANDLE
