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
/// \file stream_socket_benchmark.cpp
///
#include "stream_socket_benchmark.hpp"

#include <algorithm>

namespace jar::com::bench {

/// \brief A benchmark case for stream socket throughput
///
/// This benchmark provides the following counters:
///   - messages per second
///   - bytes per second
BENCHMARK_DEFINE_F(stream_socket_benchmark, throughput)(::benchmark::State& state)
{
  using ::benchmark::Counter;

  std::int64_t bytes{0}, messages{0};
  ipc::stream_socket client;
  client.connect(server_address());

  for (auto _ : state) {
    state.PauseTiming();
    auto data = generate();
    messages += 2;
    state.ResumeTiming();

    auto const bytes_send = client.send(data.data(), data.size());
    auto const bytes_received = client.receive(&data[0], data.size());

    assert(bytes_send == data.size());
    assert(bytes_received == data.size());

    bytes += bytes_send + bytes_received;
  }

  state.counters["Bytes"] = Counter(bytes, ::benchmark::Counter::kIsRate, Counter::kIs1024);
  state.counters["Messages"] = Counter(messages, Counter::kIsRate, Counter::kIs1000);
}

/// \brief A benchmark configuration for throughput with message sizes between 64 and 4096 bytes
///
/// This benchmark configuration provides the following custom statistics:
///   - minimum duration
///   - maximum duration
BENCHMARK_REGISTER_F(stream_socket_benchmark, throughput)
    ->ComputeStatistics("max",
                        [](const std::vector<double>& elapsed) -> double {
                          return *(std::max_element(std::begin(elapsed), std::end(elapsed)));
                        })
    ->ComputeStatistics("min",
                        [](const std::vector<double>& elapsed) -> double {
                          return *(std::min_element(std::begin(elapsed), std::end(elapsed)));
                        })
    ->RangeMultiplier(2)
    ->Range(64, 4096)
    ->Iterations(1'000'000);

}  // namespace jar::com::bench
