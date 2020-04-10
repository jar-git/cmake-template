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
/// \file stream_socket_benchmark.hpp
///
#ifndef JAR_NET_STREAM_SOCKET_BENCHMARK_HPP
#define JAR_NET_STREAM_SOCKET_BENCHMARK_HPP

#include <thread>

#include <jar/net/domain_address.hpp>
#include <jar/net/stream_server_socket.hpp>
#include <jar/net/stream_socket.hpp>

#include "basic_socket_benchmark.hpp"

namespace jar::net::bench {

/// \brief Benchmark fixture class for stream sockets
class stream_socket_benchmark : public basic_socket_benchmark {
public:
  /// \brief Sets up the test fixture
  ///
  /// \param[in|out]  state       Benchmark state
  void SetUp(::benchmark::State& state) override
  {
    start_server();
    connect_client();

    basic_socket_benchmark::SetUp(state);
  }

  /// \brief Tears down the test fixture
  ///
  /// \param[in|out]  state       Benchmark state
  void TearDown(::benchmark::State& state) override
  {
    close_client();
    close_server();

    basic_socket_benchmark::TearDown(state);
  }

  /// \brief Gets the client socket
  stream_socket& client() noexcept { return m_client_socket; }

private:
  void close_client()
  {
    m_client_socket.shutdown();
    m_client_socket.close();
  }

  void connect_client()
  {
    if (!m_client_socket.is_open()) {
      m_client_socket.open();
    }
    m_client_socket.connect(m_server_address);
  }

  void close_server()
  {
    m_server_socket.shutdown();
    m_server_socket.close();
    if (m_server_thread.joinable()) {
      m_server_thread.join();
    }
  }

  void start_server()
  {
    if (!m_server_socket.is_open()) {
      m_server_socket.open();
    }
    m_server_socket.bind(m_server_address);
    m_server_socket.listen();

    m_server_thread = std::thread{[this]() {
      m_server_socket.accept([this](stream_socket&& client_socket) {
        std::array<std::uint8_t, 4096U> buffer{};
        while (client_socket.receive(&buffer[0], message_size()) != 0) {
          client_socket.send(buffer.data(), message_size());
        }
        client_socket.shutdown();
        client_socket.close();
      });
    }};
  }

  stream_server_socket m_server_socket{socket_family::domain};
  domain_address m_server_address{SOCKET_ADDRESS};
  stream_socket m_client_socket{socket_family::domain};
  std::thread m_server_thread;
};

}  // namespace jar::net::bench

#endif  // JAR_NET_STREAM_SOCKET_BENCHMARK_HPP
