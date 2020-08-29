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

#ifndef YOUTUBE_HERMES_CONFIG_SUBSCRIBER_SIMULATION_H
#define YOUTUBE_HERMES_CONFIG_SUBSCRIBER_SIMULATION_H

#include <google/protobuf/stubs/statusor.h>
#include <grpc++/grpc++.h>

#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <queue>
#include <functional>

#include "models/EnqueueRule.h"
#include "models/EnqueueSignal.h"
#include "models/Queue.h"
#include "models/RoutingSignal.h"
#include "models/VerdictSignal.h"
#include "models/Video.h"
#include "signal.h"
#include "reviewer.h"

#include "spanner_handler.h"
#include "simulation_output.h"

#include "google/pubsub/v1/pubsub.grpc.pb.h"
#include "proto/config_change.pb.h"
#include "proto/impact_analysis_response.pb.h"
#include "google/pubsub/v1/pubsub.grpc.pb.h"
#include "absl/strings/string_view.h"
#include "absl/time/time.h"

namespace youtube_hermes_config_subscriber {
  // Vectors
  using EnqueueSignals = std::vector<EnqueueSignal>;
  using RoutingSignals = std::vector<RoutingSignal>;
  using VerdictSignals = std::vector<VerdictSignal>;
  using Videos = std::vector<Video>;
  using Queues = std::vector<EntityQueue>;
  using EnqueueRules = std::vector<EnqueueRule>;

  using Features = std::vector<std::string>;

  //Maps
  using EnqueueSignalMap = std::map<std::string, EnqueueSignal*>;
  using RoutingSignalMap = std::map<std::string, RoutingSignal*>;
  using VerdictSignalMap = std::map<std::string, VerdictSignal*>;
  using VideoMap = std::map<std::string, Video*>;
  using QueueMap = std::map<std::string, EntityQueue*>;

  using google::cloud::spanner::v1::Timestamp;

  // This function takes a list of features and a list of EnqueueRules and returns 
  // a referance to the EnqueueRule which best matches with the list of features. 
  // The EnqueueRules must be sorted from lowest to highest priority.
  std::optional<EnqueueRule> GetEnqueueRuleFromFeatures(const Features& features, EnqueueRules& enqueue_rules);
  // Gets the EnqueueRule matching the exact features
  void RemoveEnqueueRuleByExactFeatures(const Features& features, EnqueueRules& enqueue_rules);


   // custom_priority_queue_MinHeap
  class CompareSignals {
   public:
    bool operator()(Signal& a, Signal& b) {
      return a.GetTimestamp() > b.GetTimestamp();
    }
  };

  class Heap : public std::priority_queue<Signal, std::vector<Signal>, CompareSignals> {
    public:
    bool remove(const Signal& value) {
      auto it = std::find(this->c.begin(), this->c.end(), value);
      if (it != this->c.end()) {
          this->c.erase(it);
          std::make_heap(this->c.begin(), this->c.end(), this->comp);
          return true;
      }

      return false;
    }
  };


  void SimulateReviews(
      EnqueueSignals& simulation_enqueue_signals,
      RoutingSignals& simulation_routing_signals,
      VerdictSignals& simulation_verdict_signals,
      VerdictSignalMap& all_verdict_signal_map,
      std::map<std::string, EntityQueue*>& all_queue_map
  );
  
  SimulationOutput SimulateRequest(EnqueueSignals enqueue_signals, ConfigChangeRequest request);
  
}  // namespace youtube_hermes_config_subscriber

#endif  // YOUTUBE_HERMES_CONFIG_SUBSCRIBER_SIMULATION_H