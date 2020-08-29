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

#ifndef YOUTUBE_HERMES_CONFIG_SUBSCRIBER_SIGNAL_H
#define YOUTUBE_HERMES_CONFIG_SUBSCRIBER_SIGNAL_H

#include <google/protobuf/stubs/statusor.h>
#include <grpc++/grpc++.h>

#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <iomanip>
#include <ctime>
#include <chrono>

#include "models/EnqueueRule.h"
#include "models/EnqueueSignal.h"
#include "models/Queue.h"
#include "models/RoutingSignal.h"
#include "models/VerdictSignal.h"
#include "models/Video.h"

#include "simulation_output.h"

#include "google/pubsub/v1/pubsub.grpc.pb.h"
#include "proto/config_change.pb.h"
#include "proto/impact_analysis_response.pb.h"
#include "google/pubsub/v1/pubsub.grpc.pb.h"
#include "absl/strings/string_view.h"
#include "absl/time/time.h"

// TODO(ballah): Refactor file into multiple smaller files.
namespace youtube_hermes_config_subscriber {
  using google::cloud::spanner::v1::Timestamp;

  class Signal {
   public:
    explicit Signal(RoutingSignal signal) : routing_signal_(signal), has_routing_(true), timestamp_(signal.create_time_) {}
    explicit Signal(EnqueueSignal signal) : enqueue_signal_(signal), has_enqueue_(true), timestamp_(signal.create_time_) {}
    explicit Signal(VerdictSignal signal) : verdict_signal_(signal), has_verdict_(true), timestamp_(signal.create_time_) {}

    explicit Signal(VerdictSignal&& signal) : verdict_signal_(signal), has_verdict_(true), timestamp_(signal.create_time_){}
    explicit Signal(RoutingSignal&& signal) : routing_signal_(signal), has_routing_(true), timestamp_(signal.create_time_) {}

    Signal() = delete;

    bool has_routing_signal() const { return has_routing; }
    bool has_enqueue_signal() const { return has_enqueue; }
    bool has_verdict_signal() const { return has_verdict; }

    friend bool operator==(const Signal& lhs, const Signal& rhs) {
      if (lhs.has_routing_signal() != rhs.has_routing_signal()) return false;
      if (lhs.has_enqueue_signal() != rhs.has_enqueue_signal()) return false;
      if (lhs.has_verdict_signal() != rhs.has_verdict_signal()) return false;
      if (lhs.GetLifeCycleId() != rhs.GetLifeCycleId()) return false;
      if (lhs.GetTimestamp() != rhs.GetTimestamp()) return false;
      
      return true;
    }

    
    Timestamp GetTimestamp() const { return timestamp_; }
    void SetTimestamp(Timestamp timestamp) { this->timestamp_ = timestamp; }

    std::string GetQueueId() const {
      if (has_routing) {
        return routing_signal_.to_queue_;
      } 
      else if (has_enqueue) {
        return enqueue_signal_.queue_match_;
      }
      else {
        return verdict_signal_.queue_id_;
      }
    }

    std::string GetLifeCycleId() const {
      if (has_routing) {
        return routing_signal_.life_cycle_id_;
      } 
      else if (has_enqueue) {
        return enqueue_signal_.life_cycle_id_;
      }
      else {
        return verdict_signal_.life_cycle_id_;
      }
    }

    RoutingSignal GetRoutingSignal() const { return routing_signal_; }
    EnqueueSignal GetEnqueueSignal() const { return enqueue_signal_; }
    VerdictSignal GetVerdictSignal() const { return verdict_signal_; }

   private:
    bool has_routing_ = false;
    bool has_enqueue_ = false;
    bool has_verdict_ = false;

    RoutingSignal routing_signal_;
    EnqueueSignal enqueue_signal_;
    VerdictSignal verdict_signal_;
    Timestamp timestamp_;
  };
}  // namespace youtube_hermes_config_subscriber

#endif  // YOUTUBE_HERMES_CONFIG_SUBSCRIBER_SIGNAL_H