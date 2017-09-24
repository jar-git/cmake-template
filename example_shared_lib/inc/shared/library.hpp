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
#ifndef SHARED_LIBRARY_HPP
#define SHARED_LIBRARY_HPP

#include <string>

/// \brief Namespace for shared library content.
namespace shared {

/// \brief Strong typed enumeration (just to test cxx_strong_enums with the
/// compiler requirements).
enum class Code : unsigned { Ok, Error };

/// \brief Class definition for a dummy class.
class library {
public:
  /// \brief Constructor initializing dummy data.
  /// \param[in] str String value contained by the class.
  library(std::string str);
  // \brief Destructor.
  ~library() = default;
  /// \brief Dummy functionality.
  /// \param[in] ext External dummy data.
  void call(const std::string &ext);

private:
  /// \brief Dummy data.
  std::string data;
};

} // namespace shared

#endif // SHARED_LIBRARY_HPP
