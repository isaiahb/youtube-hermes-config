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
#include "simulation.h"
#include "spanner_handler.h"
#include "simulation_output.h"
#include "signal.h"
#include "reviewer.h"

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
  std::optional<EnqueueRule> GetEnqueueRuleFromFeatures(const Features& features, EnqueueRules& enqueue_rules) {
    for (EnqueueRule& enqueue_rule : enqueue_rules) {
      if (enqueue_rule.DoFeaturesMatch(features)) {
        return std::optional<std::reference_wrapper<EnqueueRule>>{enqueue_rule};
      }
    }
    return {};
  }

  // Remove the enqueue_rule from the list using features as a "primary key".
  void RemoveEnqueueRuleByExactFeatures(const Features& features, EnqueueRules& enqueue_rules) {
    auto& it = enqueue_rules.begin();
    while (it != enqueue_rules.end()) {
      if (enqueue_rule.IsFeaturesExactMatch(features)) {
        enqueue_rules.erase(it);
        return;
      }
    }
  }
  

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
    std::cout<<"Simulating Reviews"<<std::endl;
    // Step 1.
    for (EnqueueSignal& enqueue_signal : simulation_enqueue_signals) {
      heap.push(Signal(enqueue_signal));
    }

    while (!heap.empty()) {
      Signal signal = heap.top();
      heap.pop();

      EntityQueue* queue = all_queue_map[signal.GetQueueId()]; 
      if (queue == nullptr) {
        // This edge case should not occur, if it does we will ignore this signal.
        std::cout<<"queue is null: "<<signal.GetQueueId()<<std::endl;
        continue;
      }

      std::optional<Reviewer> availible_reviewer = queue->GetAvailableReviewer(signal.GetTimestamp());
      if (!availible_reviewer.has_value()) {
        signal.SetTimestamp(queue->next_availible_reviewer_time);
        heap.push(signal);
      }
      else {
        std::cout<<"Simulate Reviews: reviewers new_signal"<<std::endl;
        std::cout<<"Simulate Reviews: life_cycle_id_: "<<signal.GetLifeCycleId()<<std::endl;

        // Some enqueue signals may not have a verdict signal. If this is the case create a verdict.
        VerdictSignal* old_verdict_signal = all_verdict_signal_map[signal.GetLifeCycleId()];
        VerdictSignal old;

        if (old_verdict_signal == nullptr) {
          old = VerdictSignal();
          old.life_cycle_id_ = signal.GetLifeCycleId();
          old.queue_id_ = signal.GetQueueId();
          std::cout<<"old verdict lifecylcle id: "<<(old_verdict_signal == nullptr ? "null" : "not null")<<std::endl;
          std::cout<<"old verdict lifecylcle id: "<<old.life_cycle_id_<<std::endl;
        }
        else {
          old = *old_verdict_signal;
        }

        Signal new_signal = availible_reviewer.value()->ReviewEntity(signal, old);
        std::cout<<"Simulate Reviews: got new signal"<<std::endl;

        if (new_signal.has_routing_signal()) {
          simulation_routing_signals.push_back(new_signal.GetRoutingSignal());
          std::cout<<"Simulate Reviews: adding to the heap"<<std::endl;
          heap.push(new_signal);
          std::cout<<"Simulate Reviews: added to heap"<<std::endl;
        }
        else if (new_signal.has_verdict_signal()) {
          std::cout<<"Simulate Reviews: adding to verdict_signals"<<std::endl;
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

    std::cout<<"Getting system entities and configurations"<<std::endl;
    // Get all entities and map them for easy/quick access.
    Videos all_videos = getAllVideos();
    std::vector<youtube_hermes_config_subscriber::EntityQueue> all_queues = getAllQueues();
    EnqueueRules all_enqueue_rules = getAllEnqueueRules();
    VerdictSignals all_verdict_signals = getAllVerdictSignals();

    QueueMap all_queue_map; // Key: QueueId
    VideoMap all_video_map; // Key: VideoId
    EnqueueSignalMap enqueue_signal_map; // Key: VideoId. enqueue_signal_map used in simulation to determine when videos are signaled into the system.
    VerdictSignalMap all_verdict_signal_map; // Key: LifeCycleId.
    all_queue_map.reserve(all_queues.size());
    all_video_map.reserve(all_videos.size());
    all_verdict_signal_map.reserve(all_verdict_signals.size());
    enqueue_signal_map.reserve(enqueue_signals.size());

    // Data determined by the simulation
    Videos simulation_videos;
    EnqueueSignals simulation_enqueue_signals;
    RoutingSignals simulation_routing_signals;
    VerdictSignals simulation_verdict_signals;
    
    // Populate all_queue_map, all_video_map, and enqueue_signal_map,
    // and use enqueue_signal list with populated all_video_map to populate simulation_videos.
    for (EntityQueue& queue : all_queues) {
      all_queue_map[queue.id_] = &queue;
    }

    for (Video& video : all_videos) {
      all_video_map[video.id_] = &video;
    }

    for (EnqueueSignal& enqueue_signal : enqueue_signals) {
      enqueue_signal_map[enqueue_signal.video_id_] = &enqueue_signal;
      simulation_videos.push_back(*(all_video_map[enqueue_signal.video_id_]));
    }
    for (VerdictSignal& verdict_signal : all_verdict_signals) {
      all_verdict_signal_map[verdict_signal.life_cycle_id_] = &verdict_signal;
    }

    std::cout<<"Updating enqueue rules"<<std::endl;
    // Update EnqueueRules
    if (request.has_enqueue_rules()) {
      for (EnqueueRules_Change enqueue_rule_change : request.enqueue_rules().changes()){
        // Create new enqueue rule and add it to our list
        if (enqueue_rule_change.method() == "Add") {
          EnqueueRule new_rule;
          new_rule.queue_id_ = enqueue_rule_change.queue();
          new_rule.priority_ = enqueue_rule_change.priority();
          for (const std::string& feature : enqueue_rule_change.features()) {
            new_rule.AddRule(feature);
          }
          all_enqueue_rules.push_back(new_rule);
        }

        // Find existing enqueue rule and remove it from our list
        if (enqueue_rule_change.method() == "Remove") {
          Features features;
          for (const std::string& feature : enqueue_rule_change.features()) {
            features.push_back(feature);
          }
          RemoveEnqueueRuleByExactFeatures(features, all_enqueue_rules);
        }

      }
    }

    std::cout<<"Updating routing targets"<<std::endl;
    // Update Routing Targetss

    // Our current implementation of Reviewer in reviwer.h, uses previous verdict signal to determine route, 
    // To improve upon this implementaton the reviewer should also check the current queues routing targets..
    if (request.has_routing_targets()) {
      for (int add_queue_id : request.routing_targets().add_queues_to_route_to()) {
        //TODO Add route to queue. & convert int to string since queue_ids are stored in spanner as strings.
      }
      for (int add_queue_id : request.routing_targets().add_queues_to_route_to()) {
        //TODO Remove route from queue.
      }
    }

    // 6. Calculate new enqueue signals.
    // Loop through all simulation videos, find which enqueue rules matches with the videos features,
    // Use the enequeue rule to determine which queue to add video too,
    // Use enqueue_signal_map to determine timestamp of enqueue signal
    
    std::cout<<"Updating simulation_enqueue_signals"<<std::endl;
    for (Video& video : simulation_videos) {
      std::optional<EnqueueRule> enqueue_rule = GetEnqueueRuleFromFeatures(video.features, all_enqueue_rules);
      
      if (!enqueue_rule.has_value()) {
        // No enqueue rule matches with our entity, skip it.
        std::cout<<"No matching Enqueue-Rule found"<<std::endl;
        continue;
      }

      map<int,EntityQueue*>::iterator it = all_queue_map.find(enqueue_rule.value()->queue_id_);
      if(it == all_queue_map.end()){
        // No queue with the queue id from our enqueue_rule. This shouldn't happen.
        // Skip it.
         
        std::cout<<"No queue with the queue id from our enqueue_rule. This shouldn't happen"<<std::endl;
        continue;
      }

      EntityQueue* queue = all_queue_map[enqueue_rule.value()->queue_id_];

      EnqueueSignal new_enqueue_signal;
      new_enqueue_signal.create_time_ = enqueue_signal_map[video.id_]->create_time_;
      new_enqueue_signal.life_cycle_id_ = enqueue_signal_map[video.id_]->life_cycle_id_;
      new_enqueue_signal.queue_match_ = queue->id_;
      new_enqueue_signal.video_id_ = video.id_;

      video.life_cycle_id_ = new_enqueue_signal.life_cycle_id_;

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
