// Copyright 2017 Jani Arola, All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#include <benchmark/benchmark.h>

#include "static/library.hpp"

// NOLINTNEXTLINE (runtime/references)
void run_static(benchmark::State& state)
{
    static_lib::library lib(10);
    while (state.KeepRunning()) {
        lib.call("test");
    }
}

// NOLINTNEXTLINE - bench
BENCHMARK(run_static);
