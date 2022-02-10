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
/// \file callback_receiver_test.cpp
///
#include <gtest/gtest.h>

#include <exception>
#include <functional>
#include <utility>

#include "jar/concurrency/mock_sender.hpp"
#include "jar/concurrency/type_traits.hpp"

#include "jar/concurrency/details/callback_receiver.hpp"

namespace jar::concurrency::details::test {

inline static constexpr auto empty_value_handler = [](int) {};
inline static constexpr auto empty_error_handler = [](std::exception_ptr) noexcept {};
inline static constexpr auto empty_cancel_handler = []() noexcept {};

TEST(callback_receiver_test, test_has_future)
{
  using receiver_type = callback_receiver<int, std::function<void(int)>, std::function<void(std::exception_ptr)>,
                                          std::function<void(void)>>;
  EXPECT_TRUE(!has_future<receiver_type>::value);
}

TEST(callback_receiver_test, test_complete)
{
  static constexpr int expected{42};
  bool is_complete{false};

  auto init = make_sender_adapter(mock_sender{}, []() {
    return expected;
  });

  auto receiver = make_callback_receiver(std::function{[&is_complete](int value) {
                                           EXPECT_EQ(expected, value);
                                           is_complete = true;
                                         }},
                                         std::function{empty_error_handler}, std::function{empty_cancel_handler});

  auto state = init.connect(std::move(receiver));
  state.start();

  EXPECT_TRUE(is_complete);
}

TEST(callback_receiver_test, test_complete_void)
{
  bool is_complete{false};

  auto init = make_sender_adapter(mock_sender{}, []() {});
  auto receiver = make_callback_receiver(std::function{[&is_complete](void) {
                                           is_complete = true;
                                         }},
                                         std::function{empty_error_handler}, std::function{empty_cancel_handler});

  auto state = init.connect(std::move(receiver));
  state.start();

  EXPECT_TRUE(is_complete);
}

TEST(callback_receiver_test, test_fail)
{
  static constexpr auto const s_expected_message{"expected test error"};
  bool is_failed{false};

  auto init = make_sender_adapter(mock_sender{}, []() -> int {
    throw std::runtime_error{s_expected_message};
  });
  auto receiver =
      make_callback_receiver(std::function{empty_value_handler}, std::function{[&is_failed](std::exception_ptr e) {
                               try {
                                 is_failed = true;
                                 std::rethrow_exception(e);
                               } catch (std::runtime_error& e) {
                                 EXPECT_EQ(std::string{s_expected_message}, std::string{e.what()});
                               }
                             }},
                             std::function{empty_cancel_handler});

  auto state = init.connect(std::move(receiver));
  state.start();

  EXPECT_TRUE(is_failed);
}

TEST(callback_receiver_test, test_cancel)
{
  bool is_canceled{false};
  auto init = make_sender_adapter(mock_sender{}, []() {
    return 0;
  });
  auto receiver = make_callback_receiver(std::function{empty_value_handler}, std::function{empty_error_handler},
                                         std::function{[&is_canceled]() {
                                           is_canceled = true;
                                         }});
  receiver.cancel();

  auto state = init.connect(std::move(receiver));
  state.start();

  EXPECT_TRUE(is_canceled);
}

}  // namespace jar::concurrency::details::test
