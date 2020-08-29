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

#ifndef YOUTUBE_HERMES_CONFIG_SUBSCRIBER_PROCESSOR_H
#define YOUTUBE_HERMES_CONFIG_SUBSCRIBER_PROCESSOR_H

#include <google/protobuf/stubs/statusor.h>

#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "proto/config_change.pb.h"
#include "google/pubsub/v1/pubsub.grpc.pb.h"
#include "absl/strings/string_view.h"

#include "publisher.h"
#include "impact.h"

namespace youtube_hermes_config_subscriber {

typedef google::protobuf::util::StatusOr<ConfigChangeRequest> MessageCallback(google::pubsub::v1::PubsubMessage const&);

// Constants used for logging data in MessageProcessor function.
const char kInvalidConfigurationWarning[] = "Invalid ConfigChangeRequest Object. ConfigChangeRequest must have oneof (enqueue_rule, routing_rule, queue_info)";
const char kParsingFailedWarning[] = "Failed to parse ConfigChangeRequest Object from String";
const char kSuccessfulParsingMessage[] = "Successfully parsed ConfigChangeRequest from message";
const char kNoRoutingTargets[] = "There are no routing targets to add or remove. Please specify route_to targets to ADD or REMMOVE";
const char kEnqueueRuleHeader[] = "-- Enqueue Rule --";
const char kRoutingRuleHeader[] = "-- Routing Rule --";
const char kQueueInfoHeader[] = "-- Queue Info --";
const char kPublisherTopicLink[] = "projects/google.com:youtube-admin-pacing-server/topics/TestImpactAnalysisResponse";

// MessageProcessor Templated function.
// Message class should be a `MockMessage` or `google::pubsub::v1::PubsubMessage`.
// This function logs the contents of messages if possible, and returns the deserialized protobuf object.
template <class Message>
google::protobuf::util::StatusOr<ConfigChangeRequest> MessageProcessor(Message const& message) {
  using google::protobuf::Map;
  using google::protobuf::util::StatusOr;
  using google::protobuf::util::Status;
  using google::protobuf::util::error::Code;
  using youtube_hermes_config_subscriber::PublishMessage;
  using youtube_hermes_config_subscriber::getDummyImpactAnalysis;
  using youtube_hermes_config_subscriber::getErrorImpactAnalysis;
  
  ConfigChangeRequest config_change_request;
  bool parsed_succesfully = config_change_request.ParseFromString(message.data());

  // If parsing fails, log error and return invalid status.
  if (!parsed_succesfully) {
    std::cout << std::endl << kParsingFailedWarning << std::endl;
    std::cout << "message.data(): " << message.data() << std::endl;
    return Status(Code::INVALID_ARGUMENT, kParsingFailedWarning);
  }

  std::cout << std::endl << kSuccessfulParsingMessage << std::endl;
  
  // Log the change request, depending on the type.
  if (config_change_request.has_enqueue_rule()) {
    // Log each EnqueueRule change.
    std::cout << kEnqueueRuleHeader << std::endl;
    for (const auto& change : config_change_request.enqueue_rule().changes()) {
      std::cout << change.DebugString() << std::endl;
    }
    PublishMessage(getImpactAnalysis(config_change_request), kPublisherTopicLink);
  } else if (config_change_request.has_routing_rule()) {
    // Log each RoutingRule change.
    std::cout << kRoutingRuleHeader << std::endl;
    for (const auto& change : config_change_request.routing_rule().changes()) {
      std::cout << change.DebugString() << std::endl;
    }
    PublishMessage(getImpactAnalysis(config_change_request), kPublisherTopicLink);
  } else if (config_change_request.has_queue_info()) {
    // Log each QueueInfo change.
    std::cout << kQueueInfoHeader << std::endl;
    for (const auto& change : config_change_request.queue_info().changes()) {
      std::cout << change.DebugString() << std::endl;
    }
    PublishMessage(getEmptyImpactAnalysis(config_change_request), kPublisherTopicLink);
  }

  return config_change_request;
}

}  // namespace youtube_hermes_config_subscriber

#endif  // YOUTUBE_HERMES_CONFIG_SUBSCRIBER_PROCESSOR_H