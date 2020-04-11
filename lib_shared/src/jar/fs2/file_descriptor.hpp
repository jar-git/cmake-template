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
/// \file descriptor.ipp
///

#ifndef JAR_FS2_FILE_DESCRIPTOR_HPP
#define JAR_FS2_FILE_DESCRIPTOR_HPP

#include <system_error>
#include <type_traits>

namespace jar::fs {

/// \brief A class that represents a file descriptor
class file_descriptor {
public:
  /// \brief Constructor
  ///
  /// \param[in]  value       Native descriptor
  constexpr explicit file_descriptor(int value) noexcept
    : m_value{value}
  {
  }

  /// \brief Deleted copy constructor
  file_descriptor(const file_descriptor&) = delete;
  /// \brief Deleted copy assignment operator
  file_descriptor& operator=(const file_descriptor&) = delete;

  /// \brief Move constructor
  ///
  /// \param[in]  other       Other instance
  file_descriptor(file_descriptor&& other) noexcept
    : m_value{other.m_value}
  {
    other.m_value = s_invalid_value;
  }

  /// \brief Move assignment operator
  ///
  /// \param[in]  other       Other instance
  ///
  /// \return This instance
  file_descriptor& operator=(file_descriptor&& other) noexcept
  {
    m_value = other.m_value;
    other.m_value = s_invalid_value;

    return *this;
  }

  /// \brief Destructor
  ~file_descriptor();

  /// \brief Inequality operator
  ///
  /// \param[in]  lh          Left hand side
  /// \param[in]  rh          Right hand side
  ///
  /// \return True if the values are not equal; otherwise false
  friend constexpr bool operator!=(const file_descriptor& lh, const file_descriptor& rh) noexcept
  {
    return lh.m_value != rh.m_value;
  }

  /// \brief Equality operator
  ///
  /// \param[in]  lh          Left hand side
  /// \param[in]  rh          Right hand side
  ///
  /// \return True if the values are equal; otherwise false
  friend constexpr bool operator==(const file_descriptor& lh, const file_descriptor& rh) noexcept
  {
    return lh.m_value == rh.m_value;
  }

  /// \brief Gets the native file descriptor
  ///
  /// \return Native descriptor
  [[nodiscard]] constexpr int native() const noexcept { return m_value; }

  /// \brief Checks if the file descriptor is open
  ///
  /// \return True if open; otherwise false
  [[nodiscard]] constexpr bool is_open() const noexcept { return s_invalid_value != m_value; }

  /// \brief Close the file descriptor
  ///
  /// \throws std::system_error
  void close();

  /// \brief Sets the non-blocking mode of the file descriptor
  ///
  /// \param[in]  mode        Non-blocking mode
  ///
  /// \throws std::system_error
  void non_blocking(bool mode);

  /// \brief Gets the non-blocking mode of the file descriptor
  ///
  /// \param[in]  mode        Non-blocking mode
  ///
  /// \throws std::system_error
  [[nodiscard]] bool is_non_blocking() const;

private:
  /// \brief Get file descriptor flags
  ///
  /// \return Returns an integer holding the flags
  [[nodiscard]] int get_descriptor_flags() const;

  /// \brief Invalid file descriptor value
  static constexpr const int s_invalid_value{-1};

  int m_value;
};

}  // namespace jar::fs

#endif  // JAR_FS2_FILE_DESCRIPTOR_HPP
