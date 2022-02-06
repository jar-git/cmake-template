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
/// \file utilities_test.cpp
///
#include <gtest/gtest.h>

#include <memory>
#include <utility>

#include "jar/concurrency/mock_sender.hpp"
#include "jar/concurrency/utilities.hpp"

namespace jar::concurrency::utilities::test {

TEST(utilities_test, test_composite_state_has_future)
{
  {
    auto sender = make_sender_adapter(mock_sender{}, [](int) {});
    auto state = sender.connect(mock_receiver<int>{}.make_delegate());
    EXPECT_FALSE(has_future<decltype(state)>::value);
  }

  auto sender = make_sender_adapter(mock_sender{}, [](int) {});
  auto state = sender.connect(value_receiver<int>{});
  EXPECT_TRUE(has_future<decltype(state)>::value);
}

TEST(utilities_test, test_complete)
{
  static constexpr int expected{42};

  auto init = make_sender_adapter(mock_sender{}, []() {
    return expected;
  });
  auto final = make_sender_adapter(std::move(init), [](int arg) {
    EXPECT_EQ(expected, arg);
    return std::make_unique<int>(arg);
  });

  mock_receiver<std::unique_ptr<int>> receiver;
  EXPECT_CALL(receiver, complete).Times(1U).WillOnce([](std::unique_ptr<int> arg) {
    ASSERT_NE(nullptr, arg);
    EXPECT_EQ(expected, *arg);
  });
  auto state = final.connect(receiver.make_delegate());
  state.start();
}

TEST(utilities_test, test_fail)
{
  mock_receiver<int> receiver;
  EXPECT_CALL(receiver, fail()).Times(1U);

  receiver_adapter adapter{receiver.make_delegate(), [](int) {}};
  adapter.fail();
}

TEST(utilities_test, test_cancel)
{
  mock_receiver<int> receiver;
  EXPECT_CALL(receiver, cancel()).Times(1U);

  receiver_adapter adapter{receiver.make_delegate(), [](int) {}};
  adapter.cancel();
}

TEST(utilities_test, test_value_receiver_complete)
{
  static constexpr int expected{42};

  auto init = make_sender_adapter(mock_sender{}, []() {
    return expected;
  });
  auto state = init.connect(value_receiver<int>{});
  state.start();

  auto future = state.get_future();
  EXPECT_EQ(expected, future.get());
}

}  // namespace jar::concurrency::utilities::test
