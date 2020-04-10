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
/// \file datagram_socket_test.cpp
///
#include "datagram_socket_test.hpp"

#include <future>

namespace jar::net::test {

TEST_F(datagram_socket_test, construct)
{
  datagram_socket socket{socket_family::domain};
  EXPECT_TRUE(socket.is_open());
}

TEST_F(datagram_socket_test, open_and_close)
{
  datagram_socket socket{socket_family::domain};

  EXPECT_NO_THROW(socket.close());
  EXPECT_FALSE(socket.is_open());

  EXPECT_NO_THROW(socket.open());
  EXPECT_TRUE(socket.is_open());

  EXPECT_NO_THROW(socket.close());
  EXPECT_FALSE(socket.is_open());
  EXPECT_THROW(socket.close(), std::system_error);
}

TEST_F(datagram_socket_test, send)
{
  domain_address address_b{DGRAM_CHANNEL_B};
  datagram_socket socket{socket_family::domain};
  socket.bind(address_b);

  auto received = std::async(std::launch::async, [this, &address_b]() {
    domain_address address_r;

    std::array<std::uint8_t, s_size> buffer{};
    EXPECT_EQ(s_size, socket_channel_a().receive(buffer.data(), buffer.size(), address_r));
    EXPECT_EQ(address_b, address_r);
    EXPECT_EQ(s_data, buffer);
  });

  socket.send(s_data.data(), s_data.size(), address_a());

  // Wait for the receive to complete in orderly fashion.
  EXPECT_NO_THROW(received.get());
}

TEST_F(datagram_socket_test, send_fail)
{
  datagram_socket socket{socket_family::domain};
  domain_address remote_address{NO_ADDRESS};

  EXPECT_THROW(
      {
        try {
          socket.send(s_data.data(), s_size, remote_address);
        } catch (const std::system_error& e) {
          EXPECT_EQ(ECONNREFUSED, e.code().value());
          throw;
        }
      },
      std::system_error);

  EXPECT_THROW(socket.send(nullptr, 1U, remote_address), std::invalid_argument);
  EXPECT_THROW(socket.send(s_data.data(), 0U, remote_address), std::invalid_argument);
}

TEST_F(datagram_socket_test, receive)
{
  domain_address address_b{DGRAM_CHANNEL_B};
  datagram_socket socket{socket_family::domain};
  socket.bind(address_b);

  auto send = std::async(std::launch::async, [this, &address_b]() {
    socket_channel_a().send(s_data.data(), s_data.size(), address_b);
  });

  domain_address address_r;
  std::array<std::uint8_t, s_size> buffer{};
  EXPECT_EQ(s_size, socket.receive(buffer.data(), buffer.size(), address_r));
  EXPECT_EQ(address_a(), address_r);
  EXPECT_EQ(s_data, buffer);

  // Wait for the receive to complete in orderly fashion.
  EXPECT_NO_THROW(send.get());
}

TEST_F(datagram_socket_test, receive_fail)
{
  domain_address remote_address;
  domain_address local_address{DGRAM_CHANNEL_B};
  datagram_socket socket{socket_family::domain};
  socket.bind(local_address);
  std::array<std::uint8_t, s_size> buffer{};

  EXPECT_THROW(socket.receive(nullptr, 1U, remote_address), std::invalid_argument);
  EXPECT_THROW(socket.receive(buffer.data(), 0U, remote_address), std::invalid_argument);
}

}  // namespace jar::net::test
