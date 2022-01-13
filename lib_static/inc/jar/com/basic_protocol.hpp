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
/// \file basic_ipc_protocol.hpp
///

#ifndef JAR_COM_IPC_BASIC_PROTOCOL_HPP
#define JAR_COM_IPC_BASIC_PROTOCOL_HPP

#include "jar/com/socket_family.hpp"
#include "jar/com/socket_protocol.hpp"
#include "jar/com/socket_type.hpp"

namespace jar::com {

/// \brief A class that defines basic protocol traits
///
/// \tparam AddressType     A system specific address type used by the protocol
/// \tparam Family          Socket family for this protocol
/// \tparam Protocol        Socket protocol
template <typename AddressType, socket_family Family, socket_protocol Protocol> class basic_protocol {
public:
  /// \brief Socket address type
  using address_type = AddressType;

  /// \brief Returns socket family
  ///
  /// \return Socket family
  constexpr static socket_family family() noexcept { return Family; }

  /// \brief Returns socket protocol
  ///
  /// \return Socket protocol
  constexpr static socket_protocol protocol() noexcept { return Protocol; }
};

}  // namespace jar::com::ipc

#endif  // JAR_COM_IPC_BASIC_PROTOCOL_HPP
