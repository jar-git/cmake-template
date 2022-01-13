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

namespace jar::com::test {

TEST_F(datagram_socket_test, construct)
{
  ipc::datagram_socket socket;
  EXPECT_TRUE(socket.is_valid());
  EXPECT_EQ(socket_family::ipc, socket.family());
  EXPECT_EQ(socket_protocol::unspecified, socket.protocol());
  EXPECT_FALSE(socket.is_non_blocking());
}

TEST_F(datagram_socket_test, send)
{
  ipc::address address_b{DGRAM_CHANNEL_B};
  ipc::datagram_socket socket;
  socket.bind(address_b);

  auto received = std::async(std::launch::async, [this, &address_b]() {
    ipc::address address_r;

    std::array<std::uint8_t, s_size> buffer{};
    EXPECT_EQ(s_size, socket_channel_a().receive_from(address_r, buffer.data(), buffer.size()));
    EXPECT_EQ(address_b, address_r);
    EXPECT_EQ(s_data, buffer);
  });

  auto bytes_send = socket.send_to(address_a(), s_data.data(), s_data.size());
  EXPECT_EQ(s_data.size(), bytes_send);

  // Wait for the receive to complete in orderly fashion.
  EXPECT_NO_THROW(received.get());
}

TEST_F(datagram_socket_test, send_fail)
{
  ipc::datagram_socket socket;
  ipc::address remote_address{NO_ADDRESS};

  EXPECT_THROW(
      {
        try {
          std::ignore = socket.send_to(remote_address, s_data.data(), s_size);
        } catch (const std::system_error& e) {
          EXPECT_EQ(ECONNREFUSED, e.code().value());
          throw;
        }
      },
      std::system_error);

  EXPECT_THROW(std::ignore = socket.send_to(remote_address, nullptr, 1U), std::invalid_argument);
  EXPECT_THROW(std::ignore = socket.send_to(remote_address, s_data.data(), 0U), std::invalid_argument);
}

TEST_F(datagram_socket_test, receive)
{
  ipc::address address_b{DGRAM_CHANNEL_B};
  ipc::datagram_socket socket;
  socket.bind(address_b);

  auto send = std::async(std::launch::async, [this, &address_b]() {
    static_cast<void>(socket_channel_a().send_to(address_b, s_data.data(), s_data.size()));
  });

  ipc::address address_r;
  std::array<std::uint8_t, s_size> buffer{};
  EXPECT_EQ(s_size, socket.receive_from(address_r, buffer.data(), buffer.size()));
  EXPECT_EQ(address_a(), address_r);
  EXPECT_EQ(s_data, buffer);

  // Wait for the receive to complete in orderly fashion.
  EXPECT_NO_THROW(send.get());
}

TEST_F(datagram_socket_test, receive_fail)
{
  ipc::address remote_address;
  ipc::address local_address{DGRAM_CHANNEL_B};
  ipc::datagram_socket socket;
  socket.bind(local_address);
  std::array<std::uint8_t, s_size> buffer{};

  EXPECT_THROW(std::ignore = socket.receive_from(remote_address, nullptr, 1U), std::invalid_argument);
  EXPECT_THROW(std::ignore = socket.receive_from(remote_address, buffer.data(), 0U), std::invalid_argument);
}

}  // namespace jar::com::test
