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
/// \file sender_adapter_test.cpp
///
#include <gtest/gtest.h>

#include <memory>
#include <utility>
#include <functional>

#include "jar/concurrency/details/sender_adapter.hpp"

#include "jar/concurrency/mock_sender.hpp"

namespace jar::concurrency::details::test {

TEST(sender_adapter_test, test_receiver_adapter_has_future)
{
  using receiver_type = receiver_adapter<mock_receiver<int>, std::function<void(int)>>;
  EXPECT_FALSE(has_future<receiver_type>::value);
}

TEST(sender_adapter_test, test_complete)
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

TEST(sender_adapter_test, test_fail)
{
  mock_receiver<int> receiver;
  EXPECT_CALL(receiver, fail()).Times(1U);

  receiver_adapter adapter{receiver.make_delegate(), [](int) {}};
  adapter.fail();
}

TEST(sender_adapter_test, test_cancel)
{
  mock_receiver<int> receiver;
  EXPECT_CALL(receiver, cancel()).Times(1U);

  receiver_adapter adapter{receiver.make_delegate(), [](int) {}};
  adapter.cancel();
}

}  // namespace jar::concurrency::details::test
