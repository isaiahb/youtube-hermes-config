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

#ifndef YOUTUBE_HERMES_CONFIG_SUBSCRIBER_REVIEWER_H
#define YOUTUBE_HERMES_CONFIG_SUBSCRIBER_REVIEWER_H

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
#include "models/RoutingSignal.h"
#include "models/VerdictSignal.h"
#include "models/Video.h"
#include "signal.h"

#include "absl/strings/string_view.h"
#include "absl/time/time.h"

// TODO(ballah): Refactor file into multiple smaller files.
namespace youtube_hermes_config_subscriber {

  class Reviewer {
   public:
    explicit Reviewer() {}

    Signal ReviewEntity(Signal& signal, VerdictSignal& old_verdict) {
      start_review_time_ = signal.GetTimestamp();
      reviewing_video_ = true;

      // check if the entity is in correct queue.
      // determine if verdict or route

      // If entity is in correct queue, create new verdict signal.
      if (signal.GetQueueId() == old_verdict.queue_id_) {
        VerdictSignal new_verdict;
        new_verdict.life_cycle_id_ = signal.GetLifeCycleId();
        new_verdict.create_time_ = GetNotBusyTime();
        new_verdict.queue_id_ = signal.GetQueueId();
        return Signal(std::move(new_verdict));
      }
      else {
        // Entity is in wrong queue so create a routing signal.
        RoutingSignal routing_signal;
        routing_signal.life_cycle_id_ = signal.GetLifeCycleId();
        routing_signal.create_time_ = GetNotBusyTime();
        routing_signal.from_queue_  = signal.GetQueueId();
        routing_signal.to_queue_  = old_verdict.queue_id_;
        return Signal(std::move(routing_signal));
      }
    }

    bool IsBusy(Timestamp timestamp) {
      if (!reviewing_video_) return false;
      return GetNotBusyTime() > timestamp;
    }

    Timestamp GetNotBusyTime() {
      using google::cloud::spanner::v1::sys_time;

      auto nano_seconds = start_review_time_.get<sys_time<std::chrono::nanoseconds>>().value();
      nano_seconds += std::chrono::minutes(average_review_minutes_);
      return google::cloud::spanner::v1::MakeTimestamp(nano_seconds).value();
    }

   private:
    bool reviewing_video_;
    Timestamp start_review_time_;
    int average_review_minutes_ = 5;

  };

}  // namespace youtube_hermes_config_subscriber

#endif  // YOUTUBE_HERMES_CONFIG_SUBSCRIBER_REVIEWER_H