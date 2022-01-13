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
/// \file enum.hpp
///

#ifndef JAR_CORE_ENUM_HPP
#define JAR_CORE_ENUM_HPP

#include "jar/core/type_traits.hpp"

namespace jar::core {

/// \brief Converts an enum value to an integral value
///
/// \param[in]  value       Enumeration value
///
/// \return Enumeration value as integral value
///
/// \tparam Enumeration     Enumeration type
template <typename Enumeration, typename = std::enable_if_t<std::is_enum_v<Enumeration>>>
[[nodiscard]] static inline auto to_integral(Enumeration value) noexcept
{
  return static_cast<std::underlying_type_t<Enumeration>>(value);
}

}  // namespace jar::core

#endif  // JAR_CORE_ENUM_HPP
