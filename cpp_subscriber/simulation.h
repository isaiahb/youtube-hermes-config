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

#include "models/EnqueueRule.h"
#include "models/EnqueueSignal.h"
#include "models/Queue.h"
#include "models/RoutingSignal.h"
#include "models/VerdictSignal.h"
#include "models/Video.h"

#include "spanner_handler.h"
#include "simulation_output.h"

#include "google/pubsub/v1/pubsub.grpc.pb.h"
#include "proto/config_change.pb.h"
#include "proto/impact_analysis_response.pb.h"
#include "google/pubsub/v1/pubsub.grpc.pb.h"
#include "absl/strings/string_view.h"
#include "absl/time/time.h"

// TODO(ballah): Refactor file into multiple smaller files.
namespace youtube_hermes_config_subscriber {
  // Vectors
  using EnqueueSignals = std::vector<EnqueueSignal>;
  using RoutingSignals = std::vector<RoutingSignal>;
  using VerdictSignals = std::vector<VerdictSignal>;
  using Videos = std::vector<Video>;
  using Queues = std::vector<Queue>;
  using EnqueueRules = std::vector<EnqueueRule>;

  using Features = std::vector<std::string>;

  //Maps
  using EnqueueSignalMap = std::map<std::string, EnqueueSignal*>;
  using RoutingSignalMap = std::map<std::string, RoutingSignal*>;
  using VerdictSignalMap = std::map<std::string, VerdictSignal*>;
  using VideoMap = std::map<std::string, Video*>;
  using QueueMap = std::map<std::string, Queue*>;

  using google::cloud::spanner::v1::Timestamp;

  // This function takes a list of features and a list of EnqueueRules and returns 
  // a referance to the EnqueueRule which best matches with the list of features. 
  // The EnqueueRules must be sorted from lowest to highest priority.
  EnqueueRule* GetEnqueueRuleFromFeatures(const Features& features, const EnqueuRules& enqueue_rules) {
    for (const EnqueueRule& enqueue_rule : enqueue_rules) {
      if (enqueue_rule.DoFeaturesMatch(features)) {
        return &enqueue_rule;
      }
    }
    return nullptr;
  }

  // Gets the EnqueueRule matching the exact features
  void RemoveEnqueueRuleByExactFeatures(const Features& features, const EnqueuRules& enqueue_rules) {
    int index = 0;
    for (const EnqueueRule& enqueue_rule : enqueue_rules) {
      if (enqueue_rule.IsFeaturesExactMatch(features)) {
        enqueue_rule.erase(enqueue_rule.begin()+index);
        return;
      }
      index++;
    }
  }
  
  class Signal {
   public:
    explicit Signal(RoutingSignal signal) : routing_signal(signal), has_routing(true), timestamp(signal.create_time) {}
    explicit Signal(EnqueueSignal signal) : enqueue_signal(signal), has_enqueue(true), timestamp(signal.create_time) {}
    explicit Signal(VerdictSignal signal) : verdict_signal(signal), has_verdict(true), timestamp(signal.create_time) {}
    
    // TODO(Ballah): Delete default constructor.

    bool has_routing_signal() { return has_routing; }
    bool has_enqueue_signal() { return has_enqueue; }
    bool has_verdict_signal() { return has_verdict; }
    
    Timestamp GetTimestamp() { return timestamp; }
    void SetTimestamp(Timestamp timestamp) { this->timestamp = timestamp; }

    std::string GetQueueId() {
      if (has_routing) {
        return routing_signal->to_queue;
      } 
      else if (has_enqueue) {
        return enqueue_signal->queue_match;
      }
      else {
        return verdict_signal->queue_id;
      }
    }

    std::string GetLifeCycleId() {
      if (has_routing) {
        return routing_signal->life_cycle_id;
      } 
      else if (has_enqueue) {
        return enqueue_signal->life_cycle_id;
      }
      else {
        return verdict_signal->life_cycle_id;
      }
    }

    RoutingSignal GetRoutingSignal(){ return routing_signal; }
    EnqueueSignal GetEnqueueSignal(){ return enqueue_signal; }
    VerdictSignal GetVerdictSignal(){ return verdict_signal; }

   private:
    bool has_routing = false;
    bool has_enqueue = false;
    bool has_verdict = false;

    RoutingSignal routing_signal;
    EnqueueSignal enqueue_signal;
    VerdictSignal verdict_signal;
    Timestamp timestamp;
  }

  // custom_priority_queue_MinHeap
  class CompareSignals{
   public:
    bool operator()(const Signal &a, const Signal &b) {
      return a.timestamp < b.timestamp;
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


  class Reviewer {
   public:
    explicit Reviewer() {}

    Signal ReviewEntity(Signal& signal, VerdictSignal& old_verdict) {
      start_review_time = signal.GetTimestamp();
      reviewing_video = true;
      // check if the entity is in correct queue.
      // determine correct verdict.

      // If entity is in correct queue, create new verdict signal.
      if (signal.GetQueueId == old_verdict.queue_id) {
        VerdictSignal new_verdict;
        new_verdict.life_cycle_id = signal.GetLifeCycleId();
        new_verdict.create_time = GetNotBusyTime();
        new_verdict.queue_id = signal.GetQueueId();
        return Signal(new_verdict);
      }
      else {
        // Entity is in wrong queue so create a routing signal.
        RouingSignal routing_signal;
        routing_signal.life_cycle_id = signal.GetLifeCycleId();
        routing_signal.create_time = GetNotBusyTime();
        routing_signal.from_queue  = signal.GetQueueId();
        routing_signal.to_queue  = old_verdict.queue_id;
        return Signal(routing_signal);
      }
    }

    bool IsBusy(Timestamp timestamp) {
      if (!reviewing_video) return false;
      return GetNotBusyTime() > timestamp;
    }

    Timestamp GetNotBusyTime() {
      return start_review_time + average_review_minutes; // TODO correctly add average_review_minutes in minutes.
    }

   private:
    bool reviewing_video;
    Timestamp start_review_time;
    int average_review_minutes = 5;

  };


  void SimulateReviews(
      EnqueueSignals& simulation_enqueue_signals,
      RoutingSignals& simulation_routing_signals,
      VerdictSignals& simulation_verdict_signals,
      VerdictSignalMap& all_verdict_signal_map,
      QueueMap& all_queue_map
  ) {
    // Steps
    /*
    1. add all new enqueue signals to heap.
    2. Pull signal from heap.
    3. Check if queue has reviewers availible.
    4.  if availible reviewers Get new signal from reviewer
          if new signal is verdict add to simulation verdict signals
          if new signal is routing signal, add signal to heap.
    5.  if not availible, get the next time a reviewer is availible. and set signals timestamp to then (update  heap).
    6. repeate untill no more signals in the heap.
    */

    Heap heap;

    // Step 1.
    for (EnqueueSignal& enqueue_signal : simulation_enqueue_signals) {
      heap.push(Signal(enqueue_signal));
    }

    while (!heap.empty()) {
      Signal signal = heap.pop();
      Queue& queue = all_queue_map[signal.GetQueueId()]; 
      Reviewer* availible_reviewer = queue.GetAvailableReviewer(signal.GetTimestamp());
      if (availible_reviewer == nullptr) {
        signal.SetTimestamp(queue.next_availible_reviewer_time);
        heap.push(signal);
      }
      else {
        Signal new_signal = availible_reviewer->ReviewEntity(signal, all_verdict_signal_map[signal.GetLifeCycleId]);
        if (new_signal.has_routing_signal) {
          simulation_routing_signals.push_back(new_signal.GetRoutingSignal());
          heap.push(new_signal);
        }
        else if (new_signal.has_verdict_signal) {
          simulation_verdict_signals.push_back(new_signal.GetVerdictSignal());
        }
      }
    }
  }

  SimulationOutput SimulateRequest(EnqueueSignals enqueue_signals, ConfigChangeRequest request) {
    /*
    1. Get all Videos to match enqueue signals.
    2. Get current Enqueue rules.
    3. Get Current Routing Rules.
    4. Update EnqueueRules/RoutingTargets based of off config  change request
    5. Get  all Queues
    6. Calculate new Enqueue Signals based off of Determined Enqueue Rules
    7. Use queue and review statistics to generate new verdict signals and new routing signals
    */

    // Get all entities and map them for easy/quick access.
    Videos all_videos = getAllVideos();
    Queues all_queues = getAllQueues();
    EnqueueRules all_enqueue_rules = getAllEnqueueRules();
    VerdictSignals all_verdict_signals = getAllVerdictSignals();

    QueueMap all_queue_map; // Key: QueueId
    VideoMap all_video_map; // Key: VideoId
    EnqueueSignalMap enqueue_signal_map; // Key: VideoId. enqueue_signal_map used in simulation to determine when videos are signaled into the system.
    VerdictSignalMap all_verdict_signal_map; // Key: LifeCycleId.

    // Data determined by the simulation
    Videos simulation_videos;
    EnqueueSignals simulation_enqueue_signals;
    RoutingSignals simulation_routing_signals;
    VerdictSignals simulation_verdict_signals;
    
    // Populate all_queue_map, all_video_map, and enqueue_signal_map,
    // and use enqueue_signal list with populated all_video_map to populate simulation_videos.
    for (Queue& queue : all_queues) {
      all_queue_map[queue.id] = &queue;
    }

    for (Video& video : all_videos) {
      all_video_map[video.id] = &video;
    }

    for (EnqueueSignal& enqueue_signal : enqueue_signals) {
      enqueue_signal_map[enqueue_signal.video_id] = &enqueue_signal;
      simulation_videos.push_back(all_video_map[enqueue_signal.video_id]);
    }
    for (VerdictSignal& verdict_signal : all_verdict_signals) {
      all_verdict_signal_map[verdict_signal.life_cycle_id] = &verdict_signal;
    }

    // Update EnqueueRules
    if (request.has_enqueue_rules()) {
      for (EnqueueRules_Change enqueue_rule_change : request.enqueue_rules.changes()){
        // Create new enqueue rule and add it to our list
        if (change.method() == "Add") {
          EnqueueRule new_rule;
          new_rule.queue_id = change.queue();
          new_rule.priority = change.priority();
          for (const std::string& feature : change.features()) {
            new_rule.rule.push_back(feature);
          }
          all_enqueue_rules.push_back(new_rule);
        }

        // Find existing enqueue rule and remove it from our list
        if (change.method() == "Remove") {
          Features features;
          for (const std::string& feature : change.features()) {
            features.push_back(feature);
          }
          RemoveEnqueueRuleByExactFeatures(features, all_enqueue_rules);
        }

      }
    }

    // Update Routing Targetss
    if (request.has_routing_targets()) {
      for (int add_queue_id : request.routing_targets().add_queues_to_route_to()) {
        //TODO (ballah): Add route to queue. & convert int to string since queue_ids are stored in spanner as strings.
      }
      for (int add_queue_id : request.routing_targets().add_queues_to_route_to()) {
        //TODO (ballah): Remove route from queue.
      }
    }

    // 6. Calculate new enqueue signals.
    // Loop through all simulation videos, find which enqueue rules matches with the videos features,
    // Use the enequeue rule to determine which queue to add video too,
    // Use enqueue_signal_map to determine timestamp of enqueue signal
    for (Video& video : simulation_videos) {
      EnqueueRule* enqueue_rule = GetEnqueueRuleFromFeatures(video.features, all_enqueue_rules);
      Queue queue* = all_queue_map[enqueue_rule->queue_id];

      EnqueueSignal new_enqueue_signal;
      new_enqueue_signal.create_time = enqueue_signal_map[video.id]->create_time;
      new_enqueue_signal.life_cycle_id = enqueue_signal_map[video.id]->life_cycle_id;
      new_enqueue_signal.queue_match = queue->id;
      new_enqueue_signal.video_id = video.id;

      video.life_cycle_id = new_enqueue_signal.life_cycle_id;

      simulation_enqueue_signals.push_back(new_enqueue_signal);
    }

    SimulateReviews( 
      simulation_enqueue_signals,
      simulation_routing_signals,
      simulation_verdict_signals,
      all_verdict_signal_map,
      all_queue_map
    );

    return SimulationOutput(simulation_enqueue_signals, simulation_routing_signals, simulation_verdict_signals);
  }

}  // namespace youtube_hermes_config_subscriber

#endif  // YOUTUBE_HERMES_CONFIG_SUBSCRIBER_SIMULATION_H