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
#ifndef ROOT_PROJECT_APPLICATION_HPP
#define ROOT_PROJECT_APPLICATION_HPP

/// \brief Application specific namespace.
#include <cstdint>

namespace app {

/// \brief Dummy application class.
class application {
public:
    /// \brief Default constructor.
    application() = default;
    /// \brief Destructor.
    ~application() = default;
    /// \brief Run the application.
    /// \return Always 0.
    int run();

private:
    /// \brief Call me.
    /// \return Always 0.
    static int call();

    std::int32_t m_unused{0};
};

} // namespace app

#endif // ROOT_PROJECT_APPLICATION_HPP
