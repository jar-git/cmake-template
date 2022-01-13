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
/// \file stream_socket_test.cpp
///
#include <jar/com/stream_socket.hpp>

#include <thread>

#include "stream_socket_test.hpp"

namespace jar::com::test {

TEST_F(stream_socket_test, construct)
{
  ipc::stream_socket socket;
  EXPECT_EQ(socket_family::ipc, socket.family());
  EXPECT_EQ(socket_protocol::unspecified, socket.protocol());
  EXPECT_FALSE(socket.is_non_blocking());
}

TEST_F(stream_socket_test, connect)
{
  auto closing_connection = async_accept([](ipc::stream_socket&& client_socket) {
    EXPECT_TRUE(client_socket.is_valid());
    EXPECT_NO_THROW(client_socket.shutdown());
  });

  ipc::stream_socket socket;
  socket.connect(server_address());

  // Wait for the connection to close in orderly fashion.
  EXPECT_NO_THROW(closing_connection.get());
}

TEST_F(stream_socket_test, connect_refused)
{
  ipc::stream_socket socket;

  EXPECT_THROW(
      {
        try {
          ipc::address no_connection{NO_ADDRESS};
          socket.connect(no_connection);
        } catch (const std::system_error& e) {
          EXPECT_EQ(ECONNREFUSED, e.code().value());
          throw;
        }
      },
      std::system_error);
}

TEST_F(stream_socket_test, send)
{
  // Use shared_ptr to avoid the limitations of the std::function (declaring variables static would also work).
  auto connect = std::make_shared<std::promise<void>>();
  auto connecting = connect->get_future();

  auto closing = async_accept([connect = std::move(connect)](ipc::stream_socket&& client_socket) mutable {
    EXPECT_TRUE(client_socket.is_valid());

    connect->set_value();

    // Check that bytes are received exactly as they are send.
    std::array<std::uint8_t, s_size> receive_buffer{};
    EXPECT_EQ(s_size, client_socket.receive(receive_buffer.data(), receive_buffer.size()));
    EXPECT_EQ(s_data, receive_buffer);

    client_socket.shutdown();
  });

  ipc::stream_socket socket;
  socket.connect(server_address());

  // Wait for the connection to be established.
  EXPECT_NO_THROW(connecting.get());

  EXPECT_EQ(s_size, socket.send(s_data.data(), s_size));

  // Wait for the connection to close in orderly fashion.
  EXPECT_NO_THROW(closing.get());
}

TEST_F(stream_socket_test, send_fail)
{
  ipc::stream_socket socket;

  // Test send while not connected.
  EXPECT_THROW(
      {
        try {
          socket.send(s_data.data(), s_size);
        } catch (const std::system_error& e) {
          EXPECT_EQ(ENOTCONN, e.code().value());
          throw;
        }
      },
      std::system_error);

  EXPECT_THROW(socket.send(nullptr, 1U), std::invalid_argument);
  EXPECT_THROW(socket.send(s_data.data(), 0U), std::invalid_argument);
}

TEST_F(stream_socket_test, receive)
{
  // Use shared_ptr to avoid the limitations of the std::function (declaring variables static would also work).
  auto connect = std::make_shared<std::promise<void>>();
  auto connecting = connect->get_future();

  auto closing = async_accept([connect = std::move(connect)](ipc::stream_socket&& client_socket) mutable {
    EXPECT_TRUE(client_socket.is_valid());

    connect->set_value();

    EXPECT_EQ(s_size, client_socket.send(s_data.data(), s_size));

    client_socket.shutdown();
  });

  ipc::stream_socket socket;
  socket.connect(server_address());

  // Wait for the connection to be established.
  EXPECT_NO_THROW(connecting.get());

  // Check that bytes are received exactly as they are send, bytes are fully received
  // even if server closes before data is read
  std::array<std::uint8_t, s_size> receive_buffer{};
  socket.receive(receive_buffer.data(), receive_buffer.size());
  EXPECT_EQ(s_data, receive_buffer);

  // Wait for the connection to close in orderly fashion.
  EXPECT_NO_THROW(closing.get());
}

TEST_F(stream_socket_test, receive_fail)
{
  ipc::stream_socket socket;
  std::array<std::uint8_t, s_size> buffer{};

  EXPECT_THROW(
      {
        try {
          socket.receive(buffer.data(), buffer.size());
        } catch (const std::system_error& e) {
          EXPECT_EQ(EINVAL, e.code().value());
          throw;
        }
      },
      std::system_error);

  EXPECT_THROW(socket.receive(nullptr, 1U), std::invalid_argument);
  EXPECT_THROW(socket.receive(buffer.data(), 0U), std::invalid_argument);
}

TEST_F(stream_socket_test, shutdown_send)
{
  // Use shared_ptr to avoid the limitations of the std::function (declaring variables static would also work).
  auto connect = std::make_shared<std::promise<void>>();
  auto connecting = connect->get_future();
  auto shutdown = std::make_shared<std::promise<void>>();

  auto closing = async_accept([connect = std::move(connect), shutdown](ipc::stream_socket&& client_socket) mutable {
    EXPECT_TRUE(client_socket.is_valid());

    connect->set_value();

    // Wait for the peer to shutdown the 'send'.
    auto shutting_down = shutdown->get_future();
    shutting_down.get();

    client_socket.shutdown();
  });

  ipc::stream_socket socket;
  socket.connect(server_address());

  // Wait for the connection to be established.
  EXPECT_NO_THROW(connecting.get());

  socket.shutdown();
  EXPECT_THROW(
      {
        try {
          socket.send(s_data.data(), s_size);
        } catch (const std::system_error& e) {
          EXPECT_EQ(EPIPE, e.code().value());
          throw;
        }
      },
      std::system_error);
  shutdown->set_value();

  // Wait for the connection to close in orderly fashion.
  EXPECT_NO_THROW(closing.get());
}

TEST_F(stream_socket_test, shutdown_receive)
{
  // Use shared_ptr to avoid the limitations of the std::function (declaring variables static would also work).
  auto connect = std::make_shared<std::promise<void>>();
  auto connecting = connect->get_future();
  auto shutdown = std::make_shared<std::promise<void>>();

  auto closing = async_accept([connect = std::move(connect), shutdown](ipc::stream_socket&& client_socket) mutable {
    EXPECT_TRUE(client_socket.is_valid());

    connect->set_value();

    // Wait for the peer to shutdown the 'receive'.
    auto shutting_down = shutdown->get_future();
    shutting_down.get();

    EXPECT_THROW(
        {
          try {
            client_socket.send(s_data.data(), s_size);
          } catch (const std::system_error& e) {
            EXPECT_EQ(EPIPE, e.code().value());
            throw;
          }
        },
        std::system_error);

    client_socket.shutdown();
  });

  ipc::stream_socket socket;
  socket.connect(server_address());

  // Wait for the connection to be established.
  EXPECT_NO_THROW(connecting.get());

  socket.shutdown();
  shutdown->set_value();

  std::array<std::uint8_t, s_size> receive_buffer{};
  EXPECT_EQ(0U, socket.receive(receive_buffer.data(), receive_buffer.size()));

  // Wait for the connection to close in orderly fashion.
  EXPECT_NO_THROW(closing.get());
}

TEST_F(stream_socket_test, send_buffer)
{
  // Create buffer that is equal to '/proc/sys/net/core/wmem_default'
  static constexpr std::size_t wmem_default{212992U};
  std::array<std::uint8_t, wmem_default> buffer{};
  std::fill(buffer.begin(), buffer.end(), 'A');

  ipc::stream_socket socket;
  socket.non_blocking(true);
  socket.connect(server_address());

  EXPECT_TRUE(socket.is_non_blocking());
  EXPECT_EQ(wmem_default, socket.get_send_buffer_size());

  // Send buffer length of data and expect that it succeeds.
  EXPECT_NO_THROW(socket.send(buffer.data(), buffer.size()));
  // Send another 12 bytes, and expect that it fails due to buffer being full.
  EXPECT_THROW(
      {
        try {
          socket.send(s_data.data(), s_size);
        } catch (const std::system_error& e) {
          EXPECT_EQ(EAGAIN, e.code().value());
          throw;
        }
      },
      std::system_error);
}

}  // namespace jar::com::test
