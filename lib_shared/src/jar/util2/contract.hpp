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
/// \file contract.hpp
///

#ifndef JAR_UTIL2_CONTRACT_HPP
#define JAR_UTIL2_CONTRACT_HPP

#include <system_error>

namespace jar::util {

/// \brief Utility class for checking pre and post conditions (defensive programming)
///
/// \tparam R   Return type of the system API (e.g. decltype(::socket(0,0,0))
/// \tparam I   Invalid value (e.g. -1)
template <typename R, R I> class contract {
public:
  /// \brief Check the return value from a system API
  ///
  /// \param[in]  return_value    Return value
  ///
  /// \throws std::system_error
  static void no_system_error(R return_value)
  {
    if (I == return_value) {
      throw std::system_error{errno, std::system_category()};
    }
  }

  /// \brief Check that pointer is not null
  ///
  /// \param[in]  value           Pointer to check
  /// \param[in]  message         Exception message
  ///
  /// \tparam T                   Pointer type
  /// \throws std::system_error
  template <typename T> static void not_null(const T* value, char const* const message)
  {
    if (nullptr == value) {
      throw std::invalid_argument{message};
    }
  }

  /// \brief Check that function pointer or std::function is not null
  ///
  /// \param[in]  value           Function pointer to check
  /// \param[in]  message         Exception message
  ///
  /// \tparam F                   Function type
  /// \throws std::system_error
  template <typename F> static void not_null(const F& value, char const* const message)
  {
    if (nullptr == value) {
      throw std::invalid_argument{message};
    }
  }

  /// \brief Check that value is not zero
  ///
  /// \param[in]  value           Value to check
  /// \param[in]  message         Exception message
  ///
  /// \tparam T                   Value type
  /// \throws std::system_error
  template <typename T> static void not_zero(T value, char const* const message)
  {
    if (T{0} == value) {
      throw std::invalid_argument{message};
    }
  }

private:
  /// \brief Private constructor to prevent instantiation
  contract() = default;
};

}  // namespace jar::util

#endif  // JAR_UTIL2_CONTRACT_HPP
