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
/// \file datagram_socket_benchmark.hpp
///
#ifndef JAR_NET_DATAGRAM_SOCKET_BENCHMARK_HPP
#define JAR_NET_DATAGRAM_SOCKET_BENCHMARK_HPP

#include "basic_socket_benchmark.hpp"

#include <future>
#include <thread>

#include <jar/com/ipc/ipc.hpp>

namespace jar::com::bench {

/// \brief Benchmark fixture class for datagram sockets
class datagram_socket_benchmark : public basic_socket_benchmark {
public:
  /// \brief Sets up the test fixture
  ///
  /// \param[in|out]  state       Benchmark state
  void SetUp(::benchmark::State& state) override
  {
    basic_socket_benchmark::SetUp(state);
    m_is_running.store(true);

    auto thread_ready = make_channel_b_thread();
    thread_ready.wait();
  }

  /// \brief Tears down the test fixture
  ///
  /// \param[in|out]  state       Benchmark state
  void TearDown(::benchmark::State& state) override
  {
    m_is_running.store(false);
    if (m_thread.joinable()) {
      m_thread.join();
    }

    basic_socket_benchmark::TearDown(state);
  }

  /// \brief Gets the ipc address b
  ipc::address const& endpoint_b() noexcept { return m_endpoint_b; }

  /// \brief Gets the ipc address a
  ipc::address const& endpoint_a() const noexcept { return m_endpoint_a; }

private:
  /// \brief Init an echoing receive / send thread for the benchmark
  ///
  /// \return A future that indicates when the setup phase of the thread is done
  std::future<void> make_channel_b_thread()
  {
    std::promise<void> thread_init;
    auto thread_ready = thread_init.get_future();

    m_thread = std::thread{[this, thread_init = std::move(thread_init)]() mutable {
      ipc::datagram_socket socket;
      socket.bind(m_endpoint_a);
      socket.set_timeout(std::chrono::milliseconds{100U});

      ipc::address endpoint_r;
      std::array<std::uint8_t, 4096U> buffer{};

      thread_init.set_value();

      try {
        while (m_is_running.load()) {
          auto const bytes_received = socket.receive_from(endpoint_r, &buffer[0], message_size());
          static_cast<void>(bytes_received);
          auto const bytes_send = socket.send_to(m_endpoint_b, buffer.data(), message_size());
          static_cast<void>(bytes_send);
        };
      } catch (const std::system_error& e) {
        contract::no_system_error_other_than(e.code().value(), ETIMEDOUT);
      }

      socket.shutdown();
    }};

    return thread_ready;
  }

  ipc::address const m_endpoint_a{DGRAM_CHANNEL_A};
  ipc::address const m_endpoint_b{DGRAM_CHANNEL_B};
  std::atomic_bool m_is_running{true};
  std::thread m_thread;
};

}  // namespace jar::com::bench

#endif  // JAR_NET_DATAGRAM_SOCKET_BENCHMARK_HPP
