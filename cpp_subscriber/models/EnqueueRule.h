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

#ifndef YOUTUBE_HERMES_CONFIG_SUBSCRIBER_MODELS_ENQUEUERULE_H
#define YOUTUBE_HERMES_CONFIG_SUBSCRIBER_MODELS_ENQUEUERULE_H

#include <string>
#include <vector>

namespace youtube_hermes_config_subscriber {

class EnqueueRule {
 public:

  // explicit EnqueueRule(std::string id, std::string queue_id, std::vector<std::string>, int priority)
  // :id(id), queue_id(queue_id), rule(rule), priority(priority) {}

  // /// No default construction.
  // EnqueueRule() = delete;

  std::string id;
  std::string queue_id;
  std::vector<std::string> rule;
  std::int64_t priority;
};

}  // namespace youtube_hermes_config_subscriber

#endif  // YOUTUBE_HERMES_CONFIG_SUBSCRIBER_MODELS_ENQUEUERULE_H