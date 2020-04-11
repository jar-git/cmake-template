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
/// \file stream_socket.hpp
///
#ifndef JAR_NET2_BASIC_STREAM_SOCKET_HPP
#define JAR_NET2_BASIC_STREAM_SOCKET_HPP

#include <functional>
#include <memory>

#include "jar/net2/basic_socket.hpp"
#include "jar/net2/socket_address.hpp"
#include "jar/net2/socket_family.hpp"

namespace jar::net {

/// \brief An interface for stream sockets
class basic_stream_socket : public virtual basic_socket {
public:
  /// \brief Handler type for new stream connections
  using handler_t = std::function<void(std::unique_ptr<basic_stream_socket>)>;

  /// \brief A class that represents connection credentials
  class credentials {
    using pid_t = int;
    using uid_t = unsigned;
    using gid_t = unsigned;

  public:
    pid_t m_process_id;
    uid_t m_process_uid;
    gid_t m_process_gid;
  };

  /// \brief Deleted copy constructor
  basic_stream_socket(const basic_stream_socket&) = delete;
  /// \brief Deleted copy assignment operator
  basic_stream_socket& operator=(const basic_stream_socket&) = delete;
  /// \brief Move constructor
  basic_stream_socket(basic_stream_socket&&) = delete;
  /// \brief Move assignment operator
  basic_stream_socket& operator=(basic_stream_socket&&) = delete;

  /// \brief Destructor
  virtual ~basic_stream_socket() = default;

  /// \brief Start listening
  ///
  /// Socket must be bound before listen.
  ///
  /// \throws std::system_error
  virtual void listen() = 0;

  /// \brief Start accepting new connections
  ///
  /// Socket must listening before accept is called.
  ///
  /// \param[in]  handler         Handler for new connections
  ///
  /// \throws std::system_error
  /// \throws std::invalid_argument
  virtual void accept(handler_t&& handler) = 0;

  /// \brief Connect the socket to a server
  ///
  /// \param[in]  remote_address  Remote endpoint address
  ///
  /// \throws std::system_error
  /// \throws std::invalid_argument
  virtual void connect(const socket_address& remote_address) = 0;

  /// \brief Gets credentials of the peer
  ///
  /// \return Process credentials of the peer
  ///
  /// \throws std::system_error
  virtual credentials get_credentials() = 0;

  /// \brief Receive bytes from the remote peer
  ///
  /// \param[out] buffer          Buffer for the received bytes
  /// \param[in]  length          Buffer length
  ///
  /// \return Zero when the peer has performed an orderly shutdown; otherwise number of bytes read
  ///
  /// \throws std::system_error
  /// \throws std::invalid_argument
  virtual std::size_t receive(std::uint8_t* buffer, std::size_t length) = 0;

  /// \brief Send bytes to the remote peer
  ///
  /// \param[in]  buffer          Buffer of containing the bytes to send
  /// \param[in]  length          Buffer length
  ///
  /// \return Number of bytes send
  ///
  /// \throws std::system_error
  /// \throws std::invalid_argument
  virtual std::size_t send(const std::uint8_t* buffer, std::size_t length) = 0;

protected:
  /// \brief Constructor
  basic_stream_socket() = default;
};

/// \brief Makes a stream socket
///
/// \param[in]  family      Socket family
///
/// \throws std::system_error
std::unique_ptr<basic_stream_socket> make_stream_socket(socket_family family);

}  // namespace jar::net

#endif  // JAR_NET2_BASIC_STREAM_SOCKET_HPP
