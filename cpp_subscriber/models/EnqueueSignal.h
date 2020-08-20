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

#ifndef YOUTUBE_HERMES_CONFIG_SUBSCRIBER_MODELS_ENQUEUESIGNAL_H
#define YOUTUBE_HERMES_CONFIG_SUBSCRIBER_MODELS_ENQUEUESIGNAL_H

#include <string>
#include <vector>
#include "google/cloud/spanner/timestamp.h"

namespace youtube_hermes_config_subscriber {

class EnqueueSignal {
 public:

  explicit EnqueueSignal(){}

  std::string life_cycle_id;
  google::cloud::spanner::v1::Timestamp create_time;
  std::string queue_match;
  std::string video_id;
  std::string violation_category;
};

}  // namespace youtube_hermes_config_subscriber

#endif  // YOUTUBE_HERMES_CONFIG_SUBSCRIBER_MODELS_ENQUEUESIGNAL_H