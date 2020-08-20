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

#include <chrono>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "absl/memory/memory.h"
#include "client.h"
#include "proto/config_change.pb.h"
#include "google/pubsub/v1/pubsub.grpc.pb.h"
#include "google/cloud/spanner/client.h"

#include "mock_message.h"
#include "processor.h"
#include "spanner_handler.h"
#include "impact.h"

const char kSubscriptionsLink[] = "projects/google.com:youtube-admin-pacing-server/subscriptions/CppBinary";
const int kSecondsToKeepClientAlive = 1200;

int main() {
  using youtube_hermes_config_subscriber::getAllVideos;
  using youtube_hermes_config_subscriber::getAllVerdictSignals;
  using youtube_hermes_config_subscriber::getAllRoutingSignals;
  using youtube_hermes_config_subscriber::getAllEnqueueSignals;
  using youtube_hermes_config_subscriber::getAllQueues;
  using youtube_hermes_config_subscriber::getAllEnqueueRules;
  using youtube_hermes_config_subscriber::getImpactAnalysis;

  ConfigChangeRequest config_change_request;
  getImpactAnalysis(config_change_request);

  // Creates a Client that polls pubsub and Runs it 
  // passing the MessageProcessor function as a callback.
  using google::pubsub::v1::PubsubMessage;
  using youtube_hermes_config_subscriber::Client;
  using youtube_hermes_config_subscriber::MessageProcessor;

  Client client = Client(kSubscriptionsLink);
  client.Run(MessageProcessor<PubsubMessage>);
  std::this_thread::sleep_for(std::chrono::seconds(kSecondsToKeepClientAlive));

  // Currently it takes around 30 seconds for the stream object in the client 
  // to close after calling this Stop method.
  // We will not need to call Stop in production,
  // in Prodoction the client will run indefinitly.
  client.Stop();
  
  client.JoinThread();
  std::cout << "Program Terminating" << std::endl;
}
