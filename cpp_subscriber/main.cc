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

#include <chrono>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "absl/memory/memory.h"
#include "client.h"
#include "proto/config_change.pb.h"
#include "google/pubsub/v1/pubsub.grpc.pb.h"
#include "google/cloud/spanner/client.h"

#include "mock_message.h"
#include "processor.h"
#include "spanner_handler.h"
#include "impact.h"

const char kSubscriptionsLink[] = "projects/google.com:youtube-admin-pacing-server/subscriptions/CppBinary";
const int kSecondsToKeepClientAlive = 1200;
void spanner();
void getQueues();

int main() {
  using youtube_hermes_config_subscriber::getAllVideos;
  using youtube_hermes_config_subscriber::getAllVerdictSignals;
  using youtube_hermes_config_subscriber::getAllRoutingSignals;
  using youtube_hermes_config_subscriber::getAllEnqueueSignals;
  using youtube_hermes_config_subscriber::getAllQueues;
  using youtube_hermes_config_subscriber::getAllEnqueueRules;
  using youtube_hermes_config_subscriber::getImpactAnalysis;

  ConfigChangeRequest config_change_request;
  getImpactAnalysis(config_change_request);

  // Creates a Client that polls pubsub and Runs it 
  // passing the MessageProcessor function as a callback.
  using google::pubsub::v1::PubsubMessage;
  using youtube_hermes_config_subscriber::Client;
  using youtube_hermes_config_subscriber::MessageProcessor;

  Client client = Client(kSubscriptionsLink);
  client.Run(MessageProcessor<PubsubMessage>);
  std::this_thread::sleep_for(std::chrono::seconds(kSecondsToKeepClientAlive));

  // Currently it takes around 30 seconds for the stream object in the client 
  // to close after calling this Stop method.
  // We will not need to call Stop in production,
  // in Prodoction the client will run indefinitly.
  client.Stop();
  
  client.JoinThread();
  std::cout << "Program Terminating" << std::endl;
}

//Project ID google.com:youtube-admin-pacing-server
const char* PROJECT_ID = "google.com:youtube-admin-pacing-server";
const char* INSTANCE_ID = "historicaltraffic";
const char* DATABASE_ID = "historical_traffic";

void spanner() {
  namespace spanner = ::google::cloud::spanner;
  auto database = spanner::Database(PROJECT_ID, INSTANCE_ID, DATABASE_ID);
  auto connection = spanner::MakeConnection(database);
  auto client = spanner::Client(connection);

  spanner::SqlStatement select("SELECT Id, QueueName FROM Queues");
  using RowType = std::tuple<std::string, std::string>;
  auto rows = client.ExecuteQuery(std::move(select));
  for (auto const& row : spanner::StreamOf<RowType>(rows)) {
    if (!row) throw std::runtime_error(row.status().message());
    std::cout << "Id: " << std::get<0>(*row) << "\t";
    std::cout << "QueueName: " << std::get<1>(*row) << "\n";
  }

  std::cout << "Query completed for [spanner_query_data]\n";
}

void getQueues() {
  namespace spanner = ::google::cloud::spanner;
  auto database = spanner::Database(PROJECT_ID, INSTANCE_ID, DATABASE_ID);
  auto connection = spanner::MakeConnection(database);
  auto client = spanner::Client(connection);
  auto rows = client.Read("Queues", spanner::KeySet::All(), {"Id", "DesiredSLA_min", "Owners", "PossibleRoutes", "QueueName"});

    // The actual type of `row` is google::cloud::StatusOr<spanner::Row>, but
    // we expect it'll most often be declared with auto like this.
    for (auto const& row : rows) {
      // Use `row` like a smart pointer; check it before dereferencing
      if (!row) {
        // `row` doesn't contain a value, so `.status()` will contain error info
        std::cerr << row.status();
        break;
      }
      // The actual type of `song` is google::cloud::StatusOr<std::string>, but
      // again we expect it'll be commonly declared with auto as we show here.
      auto id = row->get<std::string>("Id");
      auto queue_name = row->get<std::string>("QueueName");
      
      // Instead of checking then dereferencing `song` as we did with `row`
      // above, here we demonstrate use of the `.value()` member, which will
      // return a reference to the contained `T` if it exists, otherwise it
      // will throw an exception (or terminate if compiled without exceptions).
      std::cout << "QueueName: " << queue_name.value();
      std::cout << ", Id: " << id.value() << "\n";
    }
}