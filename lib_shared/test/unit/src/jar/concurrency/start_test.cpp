/// Copyright 2022 Jani Arola, All rights reserved.
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
/// \file start_test.cpp
///
#include <gtest/gtest.h>

#include <future>
#include <memory>

#include "jar/concurrency/rr_scheduler.hpp"
#include "jar/concurrency/schedule.hpp"
#include "jar/concurrency/start.hpp"
#include "jar/concurrency/then.hpp"
#include "jar/concurrency/thread_pool.hpp"

namespace jar::concurrency::test {

class start_test : public ::testing::Test {
public:
  auto get_scheduler() { return m_executor.get_scheduler(); }

private:
  thread_pool<rr_scheduler> m_executor;
};

TEST_F(start_test, test_complete)
{
  auto set_done = std::make_shared<std::promise<void>>();
  auto done = set_done->get_future();

  auto action = then(schedule(get_scheduler()), [set_done = std::move(set_done)]() mutable {
    set_done->set_value();
  });

  start(std::move(action));
  EXPECT_NO_THROW(done.get());
}

}  // namespace jar::concurrency::test
