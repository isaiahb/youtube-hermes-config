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

  class Reviewer {
   public:
    explicit Reviewer(Queue* queue) : queue(queue){}
    Queue* queue;

    void Update(const Timestamp& timestamp, QueueMap& queue_map, RoutingSignals& routing_signals, VerdictSignals& verdict_signals) {
      if (reviewing_video == nullptr) {
        // Check that there are videos to take from the queue.
        if (queue->review_queue.empty()) return;

        // We're free to take a video from the queue to review.
        reviewing_video = queue->review_queue.front();
        review_queue.pop();

        // TODO(ballah): check when video was enqueued, keep track of last_review_finished time 
        // and determine correct timestamp to use for start_review_time.
        start_review_time = timestamp;
      } else {
        // TODO(ballah): determine correct way to add minutes to timestamp before comparing
        if ((timestamp + average_review_minutes) <= timestamp) {
          // Enough time has past too handle review entitiy.
          // Create verdict or routing signal.
          VerdictSignal verdict_signal;
          verdict_signal.life_cycle_id = 
        }
      }
    }


   private:
    Video* reviewing_video;
    Timestamp start_review_time;
    int average_review_minutes = 5;

  };

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

    QueueMap all_queue_map; // Key: QueueId
    VideoMap all_video_map; // Key: VideoId
    EnqueueSignalMap enqueue_signal_map; // Key: VideoId. enqueue_signal_map used in simulation to determine when videos are signaled into the system.

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

    // calculate routing signals and verdict signals
    // Have reviewer object that hold  which queue they are responsible for
    // Queue can have list of videos enqueued
    // Reviewers can have access to SimulationVariables
    // Update reviewers
    /*

      loop through all queues, call update_queue function with timestamp
      (check if any reviewers finished reviewing videos first and move videos to routing target queues, 
      and create verdict signals if verdict signals)
      (determine if queue has videos to review and check if reveiwers are availible)

      loop through all reviewers calling update-reveiwer with timestamp
      if reviewer has no video to review, 
        take a video (if queue has videos)

      if reviewer has a video, 
        check finish_review_timestamp
        if finish_review_timestamp <= current_simulation_timestamp
          determine (routing or verdict signal)
          if (routing signal)
            send video to routing target
          else
            create verdict signal
    */


  }

// class Simulation {
//  public:
//   explicit Simulation(std::vector<EnqueueSignal> enqueue_signals, ConfigChangeRequest request);

//   /// No default construction. Use `Client(std::string pubsub_subscription_link)`
//   Client() = delete;
 
// };
}  // namespace youtube_hermes_config_subscriber

#endif  // YOUTUBE_HERMES_CONFIG_SUBSCRIBER_SIMULATION_H