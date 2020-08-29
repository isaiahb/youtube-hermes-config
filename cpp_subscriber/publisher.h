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

#ifndef YOUTUBE_HERMES_CONFIG_SUBSCRIBER_PUBLISHER_H
#define YOUTUBE_HERMES_CONFIG_SUBSCRIBER_PUBLISHER_H

#include <google/protobuf/stubs/statusor.h>
#include <grpc++/grpc++.h>

#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "google/pubsub/v1/pubsub.grpc.pb.h"
#include "proto/config_change.pb.h"
#include "google/pubsub/v1/pubsub.grpc.pb.h"
#include "absl/strings/string_view.h"

namespace youtube_hermes_config_subscriber {

// Generates and returns a serialized Impact Analysis Response protobuf object.
std::string getDummyImpactAnalysis(const ConfigChangeRequest&);
std::string getEmptyImpactAnalysis(const ConfigChangeRequest&);
std::string getErrorImpactAnalysis(const ConfigChangeRequest&, const std::string&);

grpc::Status PublishMessage(const std::string& message_data, const std::string& topic);

}  // namespace youtube_hermes_config_subscriber

#endif  // YOUTUBE_HERMES_CONFIG_SUBSCRIBER_PUBLISHER_H