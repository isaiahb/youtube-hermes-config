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

#ifndef YOUTUBE_HERMES_CONFIG_SUBSCRIBER_SIMULATIONOUTPUT_H
#define YOUTUBE_HERMES_CONFIG_SUBSCRIBER_SIMULATIONOUTPUT_H

#include <string>
#include <vector>

#include "absl/strings/string_view.h"

#include "models/EnqueueSignal.h"
#include "models/RoutingSignal.h"
#include "models/VerdictSignal.h"

namespace youtube_hermes_config_subscriber {

  class SimulationOutput {
  public:
    explicit SimulationOutput() {}
    explicit SimulationOutput(
        std::vector<EnqueueSignal> enqueue_signals_,
        std::vector<RoutingSignal> routing_signals_,
        std::vector<VerdictSignal> verdict_signals_)
        : enqueue_signals(enqueue_signals_), routing_signals(routing_signals_), verdict_signals(verdict_signals_) {}

    std::vector<EnqueueSignal> enqueue_signals;
    std::vector<RoutingSignal> routing_signals;
    std::vector<VerdictSignal> verdict_signals;
  };

} // namespace youtube_hermes_config_subscriber

#endif // YOUTUBE_HERMES_CONFIG_SUBSCRIBER_SIMULATIONOUTPUT_H