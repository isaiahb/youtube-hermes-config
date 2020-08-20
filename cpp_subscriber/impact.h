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

#include "models/EnqueueRule.h"
#include "models/EnqueueSignal.h"
#include "models/Queue.h"
#include "models/RoutingSignal.h"
#include "models/VerdictSignal.h"
#include "models/Video.h"

#include "spanner_handler.h"

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
        Queue queue, 
        std::vector<EnqueueSignal> enqueue_signals,
        std::vector<RoutingSignal> routing_signals,
        std::vector<VerdictSignal> verdict_signals) {
    
    using google::cloud::spanner::v1::sys_time;
    std::vector<VerdictSignal> q_verdict_signals;
    
    for (auto signal : verdict_signals) {
        if (signal.queue_id == queue.id) {
            q_verdict_signals.push_back(signal);
        }
    }

    double average_sla_min = 0;
    for (VerdictSignal verdict : q_verdict_signals) {
        if (verdict.queue_id == queue.id) {
            std::string life_cycle_id = verdict.life_cycle_id;
            // The routing signal with the latest timestamp in the life cycle id.
            RoutingSignal routing;
            // The enqueue signal for the life cycle id.
            EnqueueSignal enqueue;
            bool routed = false;

            for (auto enqueue_signal : enqueue_signals) {
                if (enqueue_signal.life_cycle_id == life_cycle_id) {
                    enqueue = enqueue_signal;
                    continue;
                }
            }

            for (auto routing_signal : routing_signals) {
                if (routing_signal.life_cycle_id == life_cycle_id) {
                    if (!routed) {
                        routing = routing_signal;
                        routed = true;
                    } 
                    else {
                        if (routing_signal.create_time > routing.create_time) {
                            routing = routing_signal;
                        }
                    }
                }
            }
            int sla = 50;
            if (routed) {
                // Two time variables below are of type:
                // NSt6chrono10time_pointINS_3_V212system_clockENS_8durationIlSt5ratioILl1ELl1000000000EEEEEE.
                auto verdict_time = verdict.create_time.get<sys_time<std::chrono::nanoseconds>>().value();
                auto last_route_time = routing.create_time.get<sys_time<std::chrono::nanoseconds>>().value();
                sla = (std::chrono::system_clock::to_time_t(verdict_time) - std::chrono::system_clock::to_time_t(last_route_time))/60;
            } 
            else {
                auto verdict_time = verdict.create_time.get<sys_time<std::chrono::nanoseconds>>().value();
                auto enqueue_time = enqueue.create_time.get<sys_time<std::chrono::nanoseconds>>().value();
                sla = (std::chrono::system_clock::to_time_t(verdict_time) - std::chrono::system_clock::to_time_t(enqueue_time))/60;
            }
            average_sla_min += (sla/q_verdict_signals.size());
        }
    }
    return average_sla_min;
}

int getNewSLA(Queue queue);

std::string getImpactAnalysis(const ConfigChangeRequest config_change_request) {
  using google::protobuf::Timestamp;

  ImpactAnalysisResponse impact_analysis;
  impact_analysis.set_allocated_request(new ConfigChangeRequest(config_change_request));

  std::vector<Queue> queues = getAllQueues();
  std::vector<EnqueueSignal> enqueue_signals = getAllEnqueueSignals();
  std::vector<RoutingSignal> routing_signals = getAllRoutingSignals();
  std::vector<VerdictSignal> verdict_signals = getAllVerdictSignals();

  for (auto queue : queues) {
    QueueImpactAnalysis* queue_impact_analysis = impact_analysis.add_queue_impact_analysis_list();
    queue_impact_analysis->set_queue_id(queue.id);
    queue_impact_analysis->set_desired_sla_min(queue.desired_SLA);

    int prev_sla = getSLA(queue, enqueue_signals, routing_signals, verdict_signals);
    queue_impact_analysis->set_previous_sla_min(prev_sla);

    //TODO (ballah): Create a function to calculate estimated new sla.
    // The function returns new enqueue/routing/verdict signal lists and we use getSLA()
    // with the new signal.
  }

  std::cout << "impact!!! " << impact_analysis.DebugString() << std::endl;
  return impact_analysis.SerializeAsString();
}
}  // namespace youtube_hermes_config_subscriber

#endif  // YOUTUBE_HERMES_CONFIG_SUBSCRIBER_IMPACT_H