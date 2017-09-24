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
#ifndef HEADER_WRAPPER_CLASS_HPP
#define HEADER_WRAPPER_CLASS_HPP

#include <string>
#include <utility>

/// \brief Namespace for header-only library content.
namespace header {

/// \brief Class definition for a dummy wrapper class.
/// \tparam T Type of the wrapped class, must be movable and implement static
/// interface 'void call(const std::string&)'.
template <typename T> class wrapper_class {
public:
  /// \brief Constructor initializing the class with the wrapped class.
  /// \param[in] arg Instance that is moved to the wrapper.
  wrapper_class(T arg) : wrapped(std::move(arg)) {
    static_assert(std::is_pointer<T>::value, "Cannot wrap around pointers.");
  }
  /// \brief Function call operator that calls the wrapped instance.
  /// \param[in] data Data passed to the wrapped instance.
  void operator()(const ::std::string &data) { wrapped.call(data); }

private:
  /// \brief Wrapped instance.
  T wrapped;
};

} // namespace header

#endif // HEADER_WRAPPER_CLASS_HPP
