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
#include <iostream>
#include <string>
#include <utility>

#include "shared/shared_library.hpp"

namespace shared {

// cppcheck-suppress passedByValue
shared_library::shared_library(std::string str)
    : data(std::move(str))
{
}

int shared_library::call(const std::string& ext)
{
    std::cout << "Shared call result: " << data << ext << std::endl;
    // Disabling static code checks.
    return std::stoi(data);  // NOLINT(fuchsia-default-arguments)
}

}  // namespace shared
