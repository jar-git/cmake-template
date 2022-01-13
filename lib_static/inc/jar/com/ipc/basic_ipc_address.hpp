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
/// \file basic_ipc_address.hpp
///

#ifndef JAR_COM_IPC_BASIC_IPC_ADDRESS_HPP
#define JAR_COM_IPC_BASIC_IPC_ADDRESS_HPP

#include "jar/com/basic_address.hpp"

namespace jar::com::ipc {

/// \brief A class representing IPC address
template <typename AddressTraits> class basic_ipc_address : public basic_address<AddressTraits> {
  /// \brief Short-hand for base type
  using basic_address_type = basic_address<AddressTraits>;

public:
  /// \brief Default constructor
  basic_ipc_address() noexcept(std::is_nothrow_default_constructible_v<basic_address_type>)
    : basic_address_type{}
  {
  }

  /// \brief Constructor
  ///
  /// \param[in]  socket_path     Socket path
  ///
  /// \throws std::invalid_argument if the given socket path is too long
  explicit basic_ipc_address(std::string_view socket_path)
    : basic_address_type{socket_path}
  {
  }

  /// \brief Unlink IPC address to free the address for usage
  ///
  /// \throws std::system_error if operation fails due to a system error
  void unlink() const {
    AddressTraits::unlink(static_cast<std::string_view>(*this)); }
};

}  // namespace jar::com::ipc

#endif  // JAR_COM_IPC_BASIC_IPC_ADDRESS_HPP
