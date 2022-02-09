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
/// \file value_receiver.cpp
///
#include <gtest/gtest.h>

#include <utility>

#include "jar/concurrency/details/value_receiver.hpp"

#include "jar/concurrency/mock_sender.hpp"
#include "jar/concurrency/type_traits.hpp"

namespace jar::concurrency::details::test {

TEST(value_receiver_test, test_has_future)
{
  EXPECT_TRUE(has_future<value_receiver<int>>::value);
}

TEST(value_receiver_test, test_complete)
{
  static constexpr int expected{42};

  auto init = make_sender_adapter(mock_sender{}, []() {
    return expected;
  });

  value_receiver<int> receiver{};
  auto future = receiver.get_future();

  auto state = init.connect(std::move(receiver));
  state.start();

  EXPECT_EQ(expected, future.get());
}

TEST(value_receiver_test, test_cancel)
{
  auto init = make_sender_adapter(mock_sender{}, []() {
    return 0;
  });
  value_receiver<int> receiver{};
  auto future = receiver.get_future();
  receiver.cancel();
  EXPECT_TRUE(receiver.is_canceled());

  auto state = init.connect(std::move(receiver));
  state.start();

  EXPECT_TRUE(future.get().is_canceled());
}

TEST(value_receiver_test, test_fail)
{
  auto init = make_sender_adapter(mock_sender{}, []() -> int {
    throw std::runtime_error{"expected test error"};
  });
  value_receiver<int> receiver{};
  auto future = receiver.get_future();

  auto state = init.connect(std::move(receiver));
  state.start();

  EXPECT_THROW(future.get(), std::runtime_error);
}

}  // namespace jar::concurrency::details::test
