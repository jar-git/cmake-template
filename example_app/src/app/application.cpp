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
#include "app/application.hpp"

#include <future>
#include <string>

#include <header/wrapper_class.hpp>
#include <shared/shared_library.hpp>
#include <static/static_library.hpp>

namespace app {

// Disable address sanitizer.
ATTRIBUTE_NO_SANITIZE_ADDRESS
int application::run()
{
    static_lib::library static_lib(1);
    // disabling string construction as an example.
    shared::shared_library shared_lib(std::string{ "5" });
    return call();
}

// Disable thread sanitizer.
ATTRIBUTE_NO_SANITIZE_THREAD
int application::call()
{
    auto future = std::async(std::launch::async, []() {
        header::wrapper_class<shared::shared_library>
            wrapped_lib(shared::shared_library(std::string{ "10" }));
        wrapped_lib.operator()(std::string{ "data" });
    });

    future.wait();

    return 0;
}

}  // namespace app
