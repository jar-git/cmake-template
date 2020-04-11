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

#include <mutex>
#include <thread>

#include <jar/net2/basic_datagram_socket.hpp>
#include <jar/net2/socket_address.hpp>

namespace jar::net::bench {

/// \brief Benchmark fixture class for datagram sockets
class datagram_socket_benchmark : public basic_socket_benchmark {
public:
  /// \brief Sets up the test fixture
  ///
  /// \param[in|out]  state       Benchmark state
  void SetUp(::benchmark::State& state) override
  {
    open_channel(*m_socket_channel_a, *m_endpoint_a);
    open_channel(*m_socket_channel_b, *m_endpoint_b);

    m_thread = std::thread{[this]() {
      auto endpoint_r{make_domain_address("")};
      std::array<std::uint8_t, 4096U> buffer{};
      std::unique_lock lock{m_mutex};

      do {
        lock.unlock();
        m_socket_channel_a->receive(&buffer[0], message_size(), *endpoint_r);
        lock.lock();

        if (m_socket_channel_a->is_open()) {
          m_socket_channel_a->send(buffer.data(), message_size(), *m_endpoint_b);
        }
      } while (m_socket_channel_a->is_open());
    }};

    basic_socket_benchmark::SetUp(state);
  }

  /// \brief Tears down the test fixture
  ///
  /// \param[in|out]  state       Benchmark state
  void TearDown(::benchmark::State& state) override
  {
    {
      std::scoped_lock lock{m_mutex};
      close_channel(*m_socket_channel_a);
    }
    close_channel(*m_socket_channel_b);

    if (m_thread.joinable()) {
      m_thread.join();
    }

    basic_socket_benchmark::TearDown(state);
  }

  /// \brief Gets the channel b datagram socket
  basic_datagram_socket& channel_b() noexcept { return *m_socket_channel_b; }

  /// \brief Gets the channel a endpoint
  const socket_address& endpoint_a() const noexcept { return *m_endpoint_a; }

private:
  static void open_channel(basic_datagram_socket& socket_channel, const socket_address& channel)
  {
    if (!socket_channel.is_open()) {
      socket_channel.open();
    }
    socket_channel.bind(channel);
  }

  static void close_channel(basic_datagram_socket& socket_channel)
  {
    socket_channel.shutdown();
    socket_channel.close();
  }

  const std::unique_ptr<socket_address> m_endpoint_a{make_domain_address(DGRAM_CHANNEL_A)};
  const std::unique_ptr<socket_address> m_endpoint_b{make_domain_address(DGRAM_CHANNEL_B)};
  std::unique_ptr<basic_datagram_socket> m_socket_channel_a{make_datagram_socket(socket_family::domain)};
  std::unique_ptr<basic_datagram_socket> m_socket_channel_b{make_datagram_socket(socket_family::domain)};
  std::thread m_thread;
  std::mutex m_mutex;
};

}  // namespace jar::net::bench

#endif  // JAR_NET_DATAGRAM_SOCKET_BENCHMARK_HPP
