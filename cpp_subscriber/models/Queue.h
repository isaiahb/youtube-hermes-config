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

#ifndef YOUTUBE_HERMES_CONFIG_SUBSCRIBER_MODELS_QUEUE_H
#define YOUTUBE_HERMES_CONFIG_SUBSCRIBER_MODELS_QUEUE_H

#include <string>
#include <vector>
#include <queue>

#include "simulation.h"

namespace youtube_hermes_config_subscriber {

class Queue {
 public:

  explicit Queue(){
    // Currently assuming every queue has 1 reviewer
    reviewers.push_back(Reviewer(this));
  }

  // Spanner properties.
  std::string id;
  std::string queue_name;
  int desired_SLA;
  std::vector<std::string> owners;
  std::vector<std::int64_t> possible_routes;

  // Simulation variables & methods.
  std::vector<Reviewer> reviewers;

  // If all the reviewers are busy, this is the next Timestamp a reveiwer will be free at.
  google::cloud::spanner::v1::Timestamp next_availible_reviewer_time;

  *Reviewer GetAvailableReviewer(google::cloud::spanner::v1::Timestamp timestamp) {
    google::cloud::spanner::v1::Timestamp next_time = timestamp;
    for (&Reviewer reviewer : reviewers) {
      if (!reviewer.IsBusy(timestamp)) {
        return &reviewer;
      } 

      if (reviewer == reviewers.at(0)) {
        next_time = reviewer.GetNotBusyTime();
      } 
      else if (reviwer.GetNotBusyTime() < next_time) {
        next_time = reviwer.GetNotBusyTime();
      }
    }

    next_availible_reviewer_time = next_time;
    return nullptr;
  }
  
};

}  // namespace youtube_hermes_config_subscriber

#endif  // YOUTUBE_HERMES_CONFIG_SUBSCRIBER_MODELS_QUEUE_H

/*
Id	STRING(MAX)	No
DesiredSLA_min	INT64	No
Owners	ARRAY<STRING(MAX)>	No
PossibleRoutes	ARRAY<INT64>	No
QueueName	STRING(MAX)	No
*/