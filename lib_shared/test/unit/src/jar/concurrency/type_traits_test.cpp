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
/// \file type_traits.cpp
///
#include <gtest/gtest.h>

#include <functional>
#include <memory>
#include <tuple>
#include <type_traits>

#include "jar/concurrency/type_traits.hpp"

namespace jar::concurrency::test {

/// \brief A test case that checks that invocable type trait works with functor type. Basically, only one of these
///  types should be needed for testing this, because they are all classes that use the operator() to be invocable.
TEST(type_trait_test, test_invocable_with_functor_types)
{
  {
    auto empty_lambda = []() {};

    using invocable_type = invocable<decltype(empty_lambda)>;
    static_assert(invocable_type::size == 0U, "invocable could not deduce number of arguments");
    static_assert(std::is_same_v<void, invocable_type::return_type>, "invocable could not deduce return type");
    static_assert(std::is_same_v<std::tuple<>, invocable_type::args_as_tuple>,
                  "invocable could not deduce argument tuple");
  }

  {
    auto value = std::make_unique<int>(42);
    auto capture_lambda = [value = std::move(value)](int arg) {
      *value = arg;
    };

    using invocable_type = invocable<decltype(capture_lambda)>;
    static_assert(invocable_type::size == 1U, "invocable could not deduce number of arguments");
    static_assert(std::is_same_v<int, invocable_type::arg<0U>::type>, "invocable could not deduce argument type");
    static_assert(std::is_same_v<void, invocable_type::return_type>, "invocable could not deduce return type");
    static_assert(std::is_same_v<std::tuple<int>, invocable_type::args_as_tuple>,
                  "invocable could not deduce argument tuple");
  }

  {
    auto value = std::make_unique<int>(42);
    auto mutable_lambda = [value = std::move(value)](short&, int*, float) mutable {
      value.reset();
      value = std::make_unique<int>(0);
      return *value;
    };

    using invocable_type = invocable<decltype(mutable_lambda)>;
    static_assert(invocable_type::size == 3U, "invocable could not deduce number of arguments");
    static_assert(std::is_same_v<short, invocable_type::arg<0U>::type>, "invocable could not deduce argument type");
    static_assert(std::is_same_v<int*, invocable_type::arg<1U>::type>, "invocable could not deduce argument type");
    static_assert(std::is_same_v<float, invocable_type::arg<2U>::type>, "invocable could not deduce argument type");
    static_assert(std::is_same_v<int, invocable_type::return_type>, "invocable could not deduce return type");
    static_assert(std::is_same_v<std::tuple<short, int*, float>, invocable_type::args_as_tuple>,
                  "invocable could not deduce argument tuple");
  }

  {
    using empty_std_function = std::function<void(void)>;

    using invocable_type = invocable<empty_std_function>;
    static_assert(invocable_type::size == 0U, "invocable could not deduce number of arguments");
    static_assert(std::is_same_v<void, invocable_type::return_type>, "invocable could not deduce return type");
    static_assert(std::is_same_v<std::tuple<>, invocable_type::args_as_tuple>,
                  "invocable could not deduce argument tuple");
  }

  {
    using function = std::function<int(int)>;

    using invocable_type = invocable<function>;
    static_assert(invocable_type::size == 1U, "invocable could not deduce number of arguments");
    static_assert(std::is_same_v<int, invocable_type::arg<0U>::type>, "invocable could not deduce argument type");
    static_assert(std::is_same_v<int, invocable_type::return_type>, "invocable could not deduce return type");
    static_assert(std::is_same_v<std::tuple<int>, invocable_type::args_as_tuple>,
                  "invocable could not deduce argument tuple");
  }

  {
    using function = std::function<int(short&, int*, float&)>;

    using invocable_type = invocable<function>;
    static_assert(invocable_type::size == 3U, "invocable could not deduce number of arguments");
    static_assert(std::is_same_v<short, invocable_type::arg<0U>::type>, "invocable could not deduce argument type");
    static_assert(std::is_same_v<int*, invocable_type::arg<1U>::type>, "invocable could not deduce argument type");
    static_assert(std::is_same_v<float, invocable_type::arg<2U>::type>, "invocable could not deduce argument type");
    static_assert(std::is_same_v<int, invocable_type::return_type>, "invocable could not deduce return type");
    static_assert(std::is_same_v<std::tuple<short, int*, float>, invocable_type::args_as_tuple>,
                  "invocable could not deduce argument tuple");
  }

  {
    class object {
    public:
      int operator()(int) const { return 0; }
    };

    using invocable_type = invocable<object>;
    static_assert(invocable_type::size == 1U, "invocable could not deduce number of arguments");
    static_assert(std::is_same_v<int, invocable_type::arg<0U>::type>, "invocable could not deduce argument type");
    static_assert(std::is_same_v<int, invocable_type::return_type>, "invocable could not deduce return type");
    static_assert(std::is_same_v<std::tuple<int>, invocable_type::args_as_tuple>,
                  "invocable could not deduce argument tuple");
  }
}

TEST(type_trait_test, test_invocable_with_function_ptr)
{
  {
    using function_ptr = void (*)(void);

    using invocable_type = invocable<function_ptr>;
    static_assert(invocable_type::size == 0U, "invocable could not deduce number of arguments");
    static_assert(std::is_same_v<void, invocable_type::return_type>, "invocable could not deduce return type");
    static_assert(std::is_same_v<std::tuple<>, invocable_type::args_as_tuple>,
                  "invocable could not deduce argument tuple");
  }

  {
    using function_ptr = int (*)(int);

    using invocable_type = invocable<function_ptr>;
    static_assert(invocable_type::size == 1U, "invocable could not deduce number of arguments");
    static_assert(std::is_same_v<int, invocable_type::arg<0U>::type>, "invocable could not deduce argument type");
    static_assert(std::is_same_v<int, invocable_type::return_type>, "invocable could not deduce return type");
    static_assert(std::is_same_v<std::tuple<int>, invocable_type::args_as_tuple>,
                  "invocable could not deduce argument tuple");
  }

  {
    using function_ptr = int (*)(short&, int*, float&);

    using invocable_type = invocable<function_ptr>;
    static_assert(invocable_type::size == 3U, "invocable could not deduce number of arguments");
    static_assert(std::is_same_v<short, invocable_type::arg<0U>::type>, "invocable could not deduce argument type");
    static_assert(std::is_same_v<int*, invocable_type::arg<1U>::type>, "invocable could not deduce argument type");
    static_assert(std::is_same_v<float, invocable_type::arg<2U>::type>, "invocable could not deduce argument type");
    static_assert(std::is_same_v<int, invocable_type::return_type>, "invocable could not deduce return type");
    static_assert(std::is_same_v<std::tuple<short, int*, float>, invocable_type::args_as_tuple>,
                  "invocable could not deduce argument tuple");
  }
}

TEST(type_trait_test, test_invocable_with_member_function_ptr)
{
  class object {
  };

  {
    using member_function_ptr = void (object::*)(void);

    using invocable_type = invocable<member_function_ptr>;
    static_assert(invocable_type::size == 0U, "invocable could not deduce number of arguments");
    static_assert(std::is_same_v<void, invocable_type::return_type>, "invocable could not deduce return type");
    static_assert(std::is_same_v<std::tuple<>, invocable_type::args_as_tuple>,
                  "invocable could not deduce argument tuple");
  }

  {
    using member_function_ptr = int (object::*)(int);

    using invocable_type = invocable<member_function_ptr>;
    static_assert(invocable_type::size == 1U, "invocable could not deduce number of arguments");
    static_assert(std::is_same_v<int, invocable_type::arg<0U>::type>, "invocable could not deduce argument type");
    static_assert(std::is_same_v<int, invocable_type::return_type>, "invocable could not deduce return type");
    static_assert(std::is_same_v<std::tuple<int>, invocable_type::args_as_tuple>,
                  "invocable could not deduce argument tuple");
  }

  {
    using member_function_ptr = int (object::*)(short&, int*, float&);

    using invocable_type = invocable<member_function_ptr>;
    static_assert(invocable_type::size == 3U, "invocable could not deduce number of arguments");
    static_assert(std::is_same_v<short, invocable_type::arg<0U>::type>, "invocable could not deduce argument type");
    static_assert(std::is_same_v<int*, invocable_type::arg<1U>::type>, "invocable could not deduce argument type");
    static_assert(std::is_same_v<float, invocable_type::arg<2U>::type>, "invocable could not deduce argument type");
    static_assert(std::is_same_v<int, invocable_type::return_type>, "invocable could not deduce return type");
    static_assert(std::is_same_v<std::tuple<short, int*, float>, invocable_type::args_as_tuple>,
                  "invocable could not deduce argument tuple");
  }
}

}  // namespace jar::concurrency::test
