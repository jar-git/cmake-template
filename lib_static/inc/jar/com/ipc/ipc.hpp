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
/// \file ipc.hpp
///

#ifndef JAR_COM_IPC_HPP
#define JAR_COM_IPC_HPP

#include "jar/com/datagram_socket.hpp"
#include "jar/com/stream_server_socket.hpp"
#include "jar/com/stream_socket.hpp"
#include "jar/com/basic_protocol.hpp"

#include "basic_ipc_address.hpp"

#if defined(__unix__)
#include "jar/system/posix/ipc_address.hpp"
#include "jar/system/posix/socket.hpp"
#endif

namespace jar::com {
namespace ipc {

#if defined(__unix__)
/// \brief Explicit instantiation declaration for ipc address
template class basic_ipc_address<system::posix::ipc_address>;

/// \brief Type alias for ipc address
using address = basic_ipc_address<system::posix::ipc_address>;

/// \brief Type alias for ipc protocol
using protocol = basic_protocol<address, socket_family::ipc, socket_protocol::unspecified>;

///// \brief Type alias for ipc datagram socket
using datagram_socket = com::datagram_socket<system::posix::socket, protocol>;

/// \brief Type alias for ipc stream socket
using stream_socket = com::stream_socket<system::posix::socket, protocol>;

/// \brief Type alias for ipc stream server socket
using stream_server_socket = com::stream_server_socket<system::posix::socket, protocol>;
#else
#error not implemented
#endif

}  // namespace ipc

#if defined(__unix__)
/// \brief Explicit instantiation declaration for ipc datagram socket
template class datagram_socket<system::posix::socket, ipc::protocol>;

/// \brief Explicit instantiation declaration for ipc stream socket
template class stream_socket<system::posix::socket, ipc::protocol>;

/// \brief Explicit instantiation declaration for ipc stream server socket
template class stream_server_socket<system::posix::socket, ipc::protocol>;
#endif

}  // namespace jar::com

#endif  // JAR_COM_IPC_HPP
