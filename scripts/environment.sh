#!/bin/sh
#
# Copyright 2017 Jani Arola, All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

# Configurable sanitizer options.
SANITIZER_LIBRARY="@SANITIZER_LIBRARY@"
SANITIZER_OPTIONS="@SANITIZER_OPTIONS@"
SANITIZER_ENVIRON=@SANITIZER_ENVIRON@

# Export LD_PRELOAD. This is required when using sanitized shared library with an unsanitized executable when using
# shared sanitizer library.
if [[ -z "$SANITIZER_LIBRARY" ]]; then
  export LD_PRELOAD=$SANITIZER_LIBRARY
fi

# Export sanitizer options if set.
if [[ -z "$SANITIZER_ENVIRON" ]] && [[ -z "$SANITIZER_ENVIRON" ]]; then
  export $SANITIZER_ENVIRON=SANITIZER_OPTIONS
else
  echo "No sanitizer options defined."
fi
