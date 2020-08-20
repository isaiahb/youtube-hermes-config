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
#include <map>

namespace youtube_hermes_config_subscriber {

class EnqueueRule {
 public:

  // Spanner Table Columns
  std::string id;
  std::string queue_id;
  std::int64_t priority;

  void SetRule(std::vector<std::string> features) {
    this->rule = features;
  }

  void AddRule(std::string feature) {
    this->rule.push_back(feature);
    rule_map[feature] = true;
  }

  // Simulation helper functions
  bool IsFeaturesExactMatch(const std::vector<std::string>& features) {
    if (rule.size() != features.size()) return false;

    for (const std::string& feature : features) {
      if (rule_map.find(feature) == rule_map.end()) {
          return false;
      }
    }
    return true;
  }

  bool DoFeaturesMatch(const std::vector<std::string>& features) {
    // Currently assuming that the rule is a list of features that must match the video,
    // and the rule does not specify "!feature".

    // Assuming if the list of features in the rule is larger then the features we are
    // attempting to match with, then the features do not match.
    if (rule.size() > features.size()) return false;

    // Determine if the list of features meet the rule requirments.
    int features_matched = 0;
    for (const std::string& feature : features) {
      if (rule_map.find(feature) != rule_map.end()) {
        features_matched++;
        if (features_matched == rule.size()) {
          return true;
        }
      }
    }

    return false;
  }

  private: 
    // Stores a map of the rule features to improve runtime speed of Mathing Features methods.
    std::map<std::string, bool> rule_map; 
    std::vector<std::string> rule;

    void UpdateRuleMap() {
      rule_map.clear();
      for (std::string feature : rule) {
        rule_map[feature] = true;
      }
    }

};

}  // namespace youtube_hermes_config_subscriber

#endif  // YOUTUBE_HERMES_CONFIG_SUBSCRIBER_MODELS_ENQUEUERULE_H