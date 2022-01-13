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
/// \file stream_socket_test.hpp

#ifndef JAR_NET_STREAM_SOCKET_TEST_HPP
#define JAR_NET_STREAM_SOCKET_TEST_HPP

#include "basic_socket_test.hpp"

#include <future>

#include <jar/com/ipc/ipc.hpp>

namespace jar::com::test {

/// \brief Test fixture for stream socket test cases
class stream_socket_test : public basic_socket_test {
protected:
  /// \brief Constructor
  stream_socket_test()
    : m_server_socket{}
    , m_server_address{SOCKET_ADDRESS}
  {
  }

  /// \brief Sets up the test fixture
  void SetUp() override
  {
    m_server_socket.bind(m_server_address);
    m_server_socket.listen();
  }

  /// \brief Tears down the test fixture
  void TearDown() override
  {
    m_server_socket.shutdown();
  }

  /// \brief Accept connections asynchronously
  ///
  /// \param[in]  handler     Handler for new connections
  ///
  /// \return Future of the asynchronously called handler
  std::future<void> async_accept(ipc::stream_server_socket::handler_type&& handler)
  {
    return std::async(std::launch::async, [this, handler = std::move(handler)]() mutable {
      // This will not return before the handler returns or socket is shutdown (or closed).
      m_server_socket.accept(std::move(handler));
    });
  }

  /// \brief Get test server address
  ///
  /// \return Address to server socket
  const ipc::address& server_address() const noexcept { return m_server_address; }

private:
  ipc::stream_server_socket m_server_socket;
  ipc::address m_server_address;
};

}  // namespace jar::com::test

#endif  // JAR_NET_STREAM_SOCKET_TEST_HPP
