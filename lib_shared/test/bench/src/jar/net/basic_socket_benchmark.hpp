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
/// \file basic_socket_benchmark.hpp
///
#ifndef JAR_NET2_BASIC_SOCKET_BENCHMARK_HPP
#define JAR_NET2_BASIC_SOCKET_BENCHMARK_HPP

#include <benchmark/benchmark.h>

#include <random>

namespace jar::net::bench {

/// \brief A base class for sockets benchmarks fixtures
///
/// Provides the functionality to generate random data according to the test setup.
class basic_socket_benchmark : public benchmark::Fixture {
  /// \brief Charset containing the possible characters that can be generated to test state.
  static constexpr std::array<std::uint8_t, 62U> s_charset{
      '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K',
      'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
      'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

public:
  /// \brief Sets up the test fixture
  ///
  /// \param[in|out]  state       Benchmark state
  void SetUp(::benchmark::State& state) override
  {
    m_message_size = state.range(0);
    Fixture::SetUp(state);
  }

  /// \brief Generate data with random bytes for benchmarking
  ///
  /// \return Data to be used with the benchmark case
  std::vector<std::uint8_t> generate()
  {
    std::vector<std::uint8_t> data;
    data.reserve(m_message_size);
    std::generate_n(std::back_inserter(data), m_message_size, [this]() {
      return s_charset[m_distribution(m_random_engine)];
    });
    return data;
  };

  /// \brief Gets the message size
  ///
  /// \return Returns message size in bytes
  std::size_t message_size() const noexcept { return m_message_size; }

private:
  std::default_random_engine m_random_engine{std::random_device{}()};
  std::uniform_int_distribution<> m_distribution{0U, s_charset.size() - 1U};
  std::size_t m_message_size;
};

}  // namespace jar::net::bench

#endif  // JAR_NET2_BASIC_SOCKET_BENCHMARK_HPP
