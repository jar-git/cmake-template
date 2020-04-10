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
/// \file datagram_socket_benchmark.cpp
///
#include "datagram_socket_benchmark.hpp"

#include <algorithm>

namespace jar::net::bench {

/// \brief A benchmark case for datagram socket throughput
///
/// This benchmark provides the following counters:
///   - messages per second
///   - bytes per second
BENCHMARK_DEFINE_F(datagram_socket_benchmark, throughput)(::benchmark::State& state)
{
  using ::benchmark::Counter;

  std::int64_t bytes{0}, items{0};

  for (auto _ : state) {
    state.PauseTiming();
    auto data = generate();
    bytes += data.size() * 2;
    ++items;
    domain_address endpoint_r;
    state.ResumeTiming();

    channel_b().send(data.data(), data.size(), endpoint_a());
    channel_b().receive(&data[0], data.size(), endpoint_r);
  }

  state.counters["Bytes"] = Counter(bytes, ::benchmark::Counter::kIsRate, Counter::kIs1024);
  state.counters["Messages"] = Counter(items, Counter::kIsRate, Counter::kIs1000);
}

/// \brief A benchmark configuration for throughput with message sizes between 64 and 4096 bytes
///
/// This benchmark configuration provides the following custom statistics:
///   - minimum duration
///   - maximum duration
BENCHMARK_REGISTER_F(datagram_socket_benchmark, throughput)
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

}  // namespace jar::net::bench
