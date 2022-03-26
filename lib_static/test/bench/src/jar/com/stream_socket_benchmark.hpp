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

#include <future>
#include <thread>

#include <jar/com/ipc/ipc.hpp>

#include "basic_socket_benchmark.hpp"

namespace jar::com::bench {

/// \brief Benchmark fixture class for stream sockets
class stream_socket_benchmark : public basic_socket_benchmark {
public:
  /// \brief Sets up the test fixture
  ///
  /// \param[in|out]  state       Benchmark state
  void SetUp(::benchmark::State& state) override
  {
    basic_socket_benchmark::SetUp(state);

    auto thread_ready = make_server_thread();
    thread_ready.wait();
  }

  /// \brief Tears down the test fixture
  ///
  /// \param[in|out]  state       Benchmark state
  void TearDown(::benchmark::State& state) override
  {
    join_server_thread();
    basic_socket_benchmark::TearDown(state);
  }

  /// \brief Gets the server ipc address
  const ipc::address& server_address() const noexcept { return m_server_address; }

private:
  /// \brief Waits for the server thread to stop
  void join_server_thread()
  {
    if (m_server_thread.joinable()) {
      m_server_thread.join();
    }
  }

  /// \brief Setups a thread with a socket server
  ///
  /// \return A future that indicates when the server is ready to accept a connection
  std::future<void> make_server_thread()
  {
    std::promise<void> thread_init;
    auto thread_ready = thread_init.get_future();

    m_server_thread = std::thread{[this, thread_init = std::move(thread_init)]() mutable {
      ipc::stream_server_socket server_socket;
      server_socket.bind(m_server_address);
      server_socket.listen();

      thread_init.set_value();

      server_socket.accept([this](ipc::stream_socket&& client) {
        std::array<std::uint8_t, 4096U> buffer{};
        while (client.receive(&buffer[0], message_size()) != 0) {
          static_cast<void>(client.send(buffer.data(), message_size()));
        }
        client.shutdown();
      });

      server_socket.shutdown();
    }};

    return thread_ready;
  }

  ipc::address const m_server_address{SOCKET_ADDRESS};
  std::thread m_server_thread;
};

}  // namespace jar::com::bench

#endif  // JAR_NET_STREAM_SOCKET_BENCHMARK_HPP
