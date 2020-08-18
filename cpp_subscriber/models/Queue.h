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

namespace youtube_hermes_config_subscriber {

class Queue {
 public:

  explicit Queue(){}

  std::string id;
  std::string queue_name;
  int desired_SLA;
  std::vector<std::string> owners;
  std::vector<std::int64_t> possible_routes;
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