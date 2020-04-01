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
#ifndef STATIC_LIBRARY_HPP
#define STATIC_LIBRARY_HPP

#include <string>

/// \brief Namespace for shared library content.
namespace static_lib {

/// \brief Class definition for a dummy class.
class library {
public:
  /// \brief Constructor initializing dummy data.
  /// \param[in] value Value contained by the class.
  explicit library(int value);
  // \brief Destructor.
  ~library() = default;
  /// \brief Dummy functionality.
  /// \param[in] ext External dummy data.
  int call(const std::string &ext);
  /// \brief Dummy functionality marked as constexpr.
  /// \param[in] n Dummy data.
  /// \return Dummy data.
  constexpr static int something(int n);

private:
  int value;
};

} // namespace static_lib

#endif // STATIC_LIBRARY_HPP
