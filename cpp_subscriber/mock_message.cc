// Copyright 2020 Google LLC
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

#include "mock_message.h"

#include <string>

#include "google/pubsub/v1/pubsub.grpc.pb.h"
#include "proto/config_change.pb.h"

namespace youtube_hermes_config_subscriber {

MockMessage::MockMessage(ConfigChangeRequest const& config) {
  config.SerializeToString(&data_);
}

std::string MockMessage::data() const {
  return data_;
}

}  // namespace youtube_hermes_config_subscriber