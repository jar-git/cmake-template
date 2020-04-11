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
/// \file datagram_socket_test.hpp
///
#ifndef JAR_NET2_DATAGRAM_SOCKET_TEST_HPP
#define JAR_NET2_DATAGRAM_SOCKET_TEST_HPP

#include "basic_socket_test.hpp"

#include <jar/net2/basic_datagram_socket.hpp>
#include <jar/net2/socket_address.hpp>

namespace jar::net::test {

/// \brief Test fixture for datagram socket test cases
class datagram_socket_test : public basic_socket_test {
protected:
  /// \brief Constructor
  datagram_socket_test()
    : m_datagram_socket{make_datagram_socket(socket_family::domain)}
    , m_address_a{make_domain_address(DGRAM_CHANNEL_A)}
  {
  }

  /// \brief Sets up the test fixture.
  void SetUp() override { m_datagram_socket->bind(*m_address_a); }

  /// \brief Tears down the test fixture.
  void TearDown() override
  {
    m_datagram_socket->shutdown();
    m_datagram_socket->close();
  }

  /// \brief Returns datagram socket bound to channel A
  ///
  /// \return Datagram socket bound to channel A
  basic_datagram_socket& socket_channel_a() noexcept { return *m_datagram_socket; }

  /// \brief Returns address for socket channel A
  ///
  /// \return Address for channel A
  const socket_address& address_a() const noexcept { return *m_address_a; }

private:
  std::unique_ptr<basic_datagram_socket> m_datagram_socket;
  std::unique_ptr<socket_address> m_address_a;
};

}  // namespace jar::net::test

#endif  // JAR_NET2_DATAGRAM_SOCKET_TEST_HPP
