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

#include "publisher.h"

#include <google/protobuf/stubs/statusor.h>
#include <grpc++/grpc++.h>

#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "google/pubsub/v1/pubsub.grpc.pb.h"
#include "proto/config_change.pb.h"
#include "proto/impact_analysis_response.pb.h"
#include "google/pubsub/v1/pubsub.grpc.pb.h"
#include "absl/strings/string_view.h"


namespace youtube_hermes_config_subscriber {

std::string getErrorImpactAnalysis(const ConfigChangeRequest config_change_request, const std::string error) {
  ImpactAnalysisResponse impact_analysis;
  impact_analysis.set_allocated_request(new ConfigChangeRequest(config_change_request));
  impact_analysis.set_error_message(error);
  return impact_analysis.SerializeAsString();
}

std::string getEmptyImpactAnalysis(const ConfigChangeRequest config_change_request) {
  ImpactAnalysisResponse impact_analysis;
  impact_analysis.set_allocated_request(new ConfigChangeRequest(config_change_request));
  return impact_analysis.SerializeAsString();
}

std::string getDummyImpactAnalysis(const ConfigChangeRequest config_change_request) {
  using google::protobuf::Timestamp;

  ImpactAnalysisResponse impact_analysis;
  impact_analysis.set_allocated_request(new ConfigChangeRequest(config_change_request));

  // Timestamp* start_time = new Timestamp(); 
  // Timestamp* end_time = new Timestamp(); 
  // start_time->set_seconds(time(NULL));
  // start_time->set_nanos(0);
  // end_time->set_seconds(time(NULL) + 216000);
  // end_time->set_nanos(0);


  for (int i = 0; i < 10; i++) {
    QueueImpactAnalysis* queue_impact_analysis = impact_analysis.add_queue_impact_analysis_list();
    queue_impact_analysis->set_queue_id(std::to_string(i));
    queue_impact_analysis->set_desired_sla_min(rand() % 60 + 60);
    queue_impact_analysis->set_previous_sla_min(rand() % 60 + 60);
    queue_impact_analysis->set_new_sla_min(rand() % 60 + 60);

    // queue_impact_analysis->set_allocated_analysis_period_start(start_time);
    // queue_impact_analysis->set_allocated_analysis_period_end(end_time);

    queue_impact_analysis->set_previous_avg_video_volume_per_hour(rand() % 1000 + 500);
    queue_impact_analysis->set_new_avg_video_volume_per_hour(rand() % 1000 + 500);
  }

  ImpactAnalysisResponse test;
  test.ParseFromString(impact_analysis.SerializeAsString());
  std::cout << "TEST!!! " << test.DebugString() << std::endl;

  return impact_analysis.SerializeAsString();
}

void PublishMessage(const std::string message_data, const std::string topic) {
  using grpc::ClientContext;
  using google::pubsub::v1::Publisher;
  using google::pubsub::v1::PublishRequest;
  using google::pubsub::v1::PublishResponse;
  using google::pubsub::v1::PubsubMessage;

  auto credentials = grpc::GoogleDefaultCredentials();
  auto channel = grpc::CreateChannel("pubsub.googleapis.com", credentials);
  std::unique_ptr<Publisher::Stub> stub(Publisher::NewStub(channel));

  PublishRequest request;
  request.set_topic(topic);
  PubsubMessage message;
  message.set_data(message_data);
  *request.add_messages() = message;

  PublishResponse response;
  ClientContext clientContext;

  auto status = stub->Publish(&clientContext, request, &response);
  if (!status.ok()) {
      std::cout << "failed" + std::to_string(status.error_code()) + ": " + status.error_message() << '\n';
  }

}

}  // namespace youtube_hermes_config_subscriber
