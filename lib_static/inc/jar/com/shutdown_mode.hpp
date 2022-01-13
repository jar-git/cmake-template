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
/// \file shutdown_mode.hpp
///

#ifndef JAR_COM_SHUTDOWN_MODE_HPP
#define JAR_COM_SHUTDOWN_MODE_HPP

#include <iosfwd>

namespace jar::com {

/// \brief An enumeration representing socket shutdown mode
enum class shutdown_mode : int {
  receive, ///< Shutdown receive
  send,    ///< Shutdown send
  both     ///< Shutdown both
};

/// \brief Stream extraction operator for shutdown mode
///
/// \param[in|out]  os      Output stream
/// \param[in]      mode    Shutdown mode
///
/// \return Output stream
std::ostream &operator<<(std::ostream &os, shutdown_mode mode);

} // namespace jar::com

#endif // JAR_COM_SHUTDOWN_MODE_HPP
