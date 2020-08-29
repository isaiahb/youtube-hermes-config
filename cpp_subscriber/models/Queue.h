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
#include <optional>

#include "signal.h"
#include "./../reviewer.h"

namespace youtube_hermes_config_subscriber {

class EntityQueue {
 public:

  explicit EntityQueue() {
    // Currently assuming every queue has 1 reviewer
    reviewers.push_back(Reviewer());  
  }

  // Spanner properties.
  std::string id_;
  std::string queue_name_;
  int desired_SLA_;
  std::vector<std::string> owners_;
  std::vector<std::int64_t> possible_routes_;

  // Simulation variables & methods.
  std::vector<Reviewer> reviewers_;

  // If all the reviewers are busy, this is the next Timestamp a reveiwer will be free at.
  google::cloud::spanner::v1::Timestamp next_availible_reviewer_time_;

  std::optional<Reviewer> GetAvailableReviewer(google::cloud::spanner::v1::Timestamp timestamp) {
    google::cloud::spanner::v1::Timestamp next_time = timestamp;
    bool time_set = false;
    for (Reviewer& reviewer : reviewers_) {
      if (!reviewer.IsBusy(timestamp)) {
        return std::optional<std::reference_wrapper<Reviewer>>{reviewer};
      } 

      if (!time_set) {
        time_set = true;
        next_time = reviewer.GetNotBusyTime();
      } 
      else if (reviewer.GetNotBusyTime() < next_time) {
        next_time = reviewer.GetNotBusyTime();
      }
    }

    next_availible_reviewer_time_ = next_time;
    return {};
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