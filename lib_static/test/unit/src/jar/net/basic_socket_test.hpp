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
/// \file basic_socket_test.hpp
///
#ifndef JAR_NET_BASIC_SOCKET_TEST_HPP
#define JAR_NET_BASIC_SOCKET_TEST_HPP

#include <gtest/gtest.h>

namespace jar::com::test {

/// \brief Test fixture for datagram socket test cases
class basic_socket_test : public ::testing::Test {
public:
  /// \brief Length of the test data
  static constexpr std::size_t s_size{11U};
  /// \brief Test data
  static constexpr std::array<std::uint8_t, s_size> s_data{0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x20,
                                                           0x77, 0x6f, 0x72, 0x6c, 0x64};
};

}  // namespace jar::com::test

#endif  // JAR_NET_BASIC_SOCKET_TEST_HPP
