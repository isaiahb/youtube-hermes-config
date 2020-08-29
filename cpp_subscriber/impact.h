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

#ifndef YOUTUBE_HERMES_CONFIG_SUBSCRIBER_IMPACT_H
#define YOUTUBE_HERMES_CONFIG_SUBSCRIBER_IMPACT_H

#include <google/protobuf/stubs/statusor.h>
#include <grpc++/grpc++.h>

#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <optional>

#include "models/EnqueueRule.h"
#include "models/EnqueueSignal.h"
#include "models/Queue.h"
#include "models/RoutingSignal.h"
#include "models/VerdictSignal.h"
#include "models/Video.h"
#include "spanner_handler.h"
#include "simulation.h"

#include "google/pubsub/v1/pubsub.grpc.pb.h"
#include "proto/config_change.pb.h"
#include "proto/impact_analysis_response.pb.h"
#include "google/pubsub/v1/pubsub.grpc.pb.h"
#include "absl/strings/string_view.h"
#include "absl/time/time.h"

namespace youtube_hermes_config_subscriber {


/*
  Takes a queue and the list of enqueue, routing, and verdict signals,
  and returns the calculated SLA for the queue.
  Can be used to calculate previous SLA by passing the current systems 
  enqueue/routing/verdict signals
  or a estimated New SLA by passing simulated signals. 
*/
int getSLA(
        const EntityQueue& queue, 
        const std::vector<EnqueueSignal>& enqueue_signals,
        const std::vector<RoutingSignal>& routing_signals,
        const std::vector<VerdictSignal>& verdict_signals) {
    
    using google::cloud::spanner::v1::sys_time;
    using google::cloud::spanner::v1::Timestamp;

    std::vector<VerdictSignal> q_verdict_signals;
    
    for (auto& signal : verdict_signals) {
        if (signal.queue_id_ == queue.id_) {
            q_verdict_signals.push_back(signal);
        }
    }

    double average_sla_min = 0;
    for (VerdictSignal verdict : q_verdict_signals) {
        std::string life_cycle_id = verdict.life_cycle_id_;
        std::optional<RoutingSignal> routing;
        Timestamp enqueue_or_route_time;

        for (auto& enqueue_signal : enqueue_signals) {
            if (enqueue_signal.life_cycle_id_ == life_cycle_id) {
                enqueue_or_route_time = enqueue_signal.create_time_;
                break;
            }
        }

        for (auto& routing_signal : routing_signals) {
            if (routing_signal.life_cycle_id_ == life_cycle_id) {

                if (!routing.has_value() || routing_signal.create_time_ > routing.value().create_time_) {
                    enqueue_or_route_time = routing_signal.create_time_;
                    routing = routing_signal;
                }
            }
        }
        
        auto verdict_time = verdict.create_time_.get<sys_time<std::chrono::nanoseconds>>().value();
        auto enqueue_time = enqueue_or_route_time.get<sys_time<std::chrono::nanoseconds>>().value();
        int sla = (std::chrono::system_clock::to_time_t(verdict_time) - std::chrono::system_clock::to_time_t(enqueue_time))/60;        
        average_sla_min += ((1.0*sla)/q_verdict_signals.size());
    }
    return average_sla_min;
}

std::string getImpactAnalysis(const ConfigChangeRequest& config_change_request) {
  using google::protobuf::Timestamp;

  ImpactAnalysisResponse impact_analysis;
  impact_analysis.set_allocated_request(new ConfigChangeRequest(config_change_request));

  std::vector<EntityQueue> queues = getAllQueues();
  std::vector<EnqueueSignal> enqueue_signals = getAllEnqueueSignals();
  std::vector<RoutingSignal> routing_signals = getAllRoutingSignals();
  std::vector<VerdictSignal> verdict_signals = getAllVerdictSignals();
  std::cout<<"starting simulation"<<std::endl;
  SimulationOutput simulation_output = SimulateRequest(enqueue_signals, config_change_request);
  std::cout<<"finished simulation"<<std::endl;

  for (auto queue : queues) {
    QueueImpactAnalysis* queue_impact_analysis = impact_analysis.add_queue_impact_analysis_list();
    queue_impact_analysis->set_queue_id(queue.id_);
    queue_impact_analysis->set_desired_sla_min(queue.desired_SLA_);

    int prev_sla = getSLA(queue, enqueue_signals, routing_signals, verdict_signals);
    queue_impact_analysis->set_previous_sla_min(prev_sla);

    int new_sla = getSLA(queue, simulation_output.enqueue_signals_, simulation_output.routing_signals_, simulation_output.verdict_signals_);
    queue_impact_analysis->set_new_sla_min(new_sla);

  }

  return impact_analysis.SerializeAsString();
}
}  // namespace youtube_hermes_config_subscriber

#endif  // YOUTUBE_HERMES_CONFIG_SUBSCRIBER_IMPACT_H