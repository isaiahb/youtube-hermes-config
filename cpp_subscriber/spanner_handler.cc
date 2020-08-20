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

#include <google/protobuf/stubs/statusor.h>
#include <grpc++/grpc++.h>

#include <sstream>
#include <string>
#include <vector>

#include "models/EnqueueRule.h"
#include "models/EnqueueSignal.h"
#include "models/Queue.h"
#include "models/RoutingSignal.h"
#include "models/VerdictSignal.h"
#include "models/Video.h"

#include "google/pubsub/v1/pubsub.grpc.pb.h"
#include "proto/config_change.pb.h"
#include "google/pubsub/v1/pubsub.grpc.pb.h"
#include "absl/strings/string_view.h"
#include "google/cloud/spanner/client.h"
#include "google/cloud/spanner/timestamp.h"

#include "spanner_handler.h"

// TODO (ballah): Create a singleton, or static variables to store the
// client connection to prevent having to create a new db connection
// every time we call a function.
namespace youtube_hermes_config_subscriber {
    const char *PROJECT_ID = "google.com:youtube-admin-pacing-server";
    const char *INSTANCE_ID = "historicaltraffic";
    const char *DATABASE_ID = "historical_traffic";

    std::vector<EnqueueRule> getAllEnqueueRules() {
        namespace spanner = ::google::cloud::spanner;
        auto database = spanner::Database(PROJECT_ID, INSTANCE_ID, DATABASE_ID);
        auto connection = spanner::MakeConnection(database);
        auto client = spanner::Client(connection);
        auto rows = client.Read("EnqueueRule", spanner::KeySet::All(),
            {"Id", "Priority", "QueueId", "Rule"});

        std::vector<EnqueueRule> list;
        // The type of `row` is google::cloud::StatusOr<spanner::Row>
        for (auto const &row : rows) {
            // Use `row` like a smart pointer; check it before dereferencing
            if (!row) {
                // `row` doesn't contain a value, so `.status()` will contain error info
                std::cout<<"!row"<<std::endl;
                std::cerr << row.status();
                break;
            }

            // The actual type of `the following varibles are:
            // google::cloud::StatusOr<T> i.e StatusOr<std::string>
            EnqueueRule object;
            object.id = row->get<std::string>("Id").value();
            object.queue_id = row->get<std::string>("QueueId").value();
            object.priority = row->get<std::int64_t>("Priority").value();
            object.setRule(row->get<std::vector<std::string>>("Rule").value());
            list.push_back(object);
        }
        return list;
    }

    std::vector<Queue> getAllQueues() {
        namespace spanner = ::google::cloud::spanner;
        auto database = spanner::Database(PROJECT_ID, INSTANCE_ID, DATABASE_ID);
        auto connection = spanner::MakeConnection(database);
        auto client = spanner::Client(connection);
        auto rows = client.Read("Queues", spanner::KeySet::All(), 
            {"Id", "DesiredSLA_min", "Owners", "PossibleRoutes", "QueueName"});
        std::vector<Queue> list;
        for (auto const& row : rows) {
            if (!row) {
                std::cerr << row.status();
                break;
            }
            auto id = row->get<std::string>("Id");
            auto desired_sla_min = row->get<std::int64_t>("DesiredSLA_min");
            auto owners = row->get<std::vector<std::string>>("Owners");
            auto possible_routes = row->get<std::vector<std::int64_t>>("PossibleRoutes");
            auto queue_name = row->get<std::string>("QueueName");

            Queue queue;
            queue.id = id.value();
            queue.desired_SLA = desired_sla_min.value();
            queue.owners = owners.value();
            queue.possible_routes = possible_routes.value();
            queue.queue_name = queue_name.value();
            list.push_back(queue);
        }
        return list;
    }

    std::vector<EnqueueSignal> getAllEnqueueSignals() {
        namespace spanner = ::google::cloud::spanner;
        auto database = spanner::Database(PROJECT_ID, INSTANCE_ID, DATABASE_ID);
        auto connection = spanner::MakeConnection(database);
        auto client = spanner::Client(connection);
        auto rows = client.Read("EnqueueSignal", spanner::KeySet::All(), 
            {"LifeCycleId", "CreateTime", "QueueMatch", "VideoId", "ViolationCategory"});
        std::vector<EnqueueSignal> list;
        for (auto const& row : rows) {
            if (!row) {
                std::cerr << row.status();
                break;
            }
            auto LifeCycleId = row->get<std::string>("LifeCycleId");
            auto CreateTime = row->get<google::cloud::spanner::v1::Timestamp>("CreateTime");
            auto QueueMatch = row->get<std::string>("QueueMatch"); //determine correct time
            auto VideoId = row->get<std::string>("VideoId");
            auto ViolationCategory = row->get<std::string>("ViolationCategory");

            EnqueueSignal enqueueSignal;
            enqueueSignal.life_cycle_id = LifeCycleId.value();
            enqueueSignal.create_time = CreateTime.value();
            enqueueSignal.queue_match = QueueMatch.value();
            enqueueSignal.video_id = VideoId.value();
            enqueueSignal.violation_category = ViolationCategory.value();
            
            list.push_back(enqueueSignal);
        }
        return list;
    }

    std::vector<RoutingSignal> getAllRoutingSignals() {
        namespace spanner = ::google::cloud::spanner;
        auto database = spanner::Database(PROJECT_ID, INSTANCE_ID, DATABASE_ID);
        auto connection = spanner::MakeConnection(database);
        auto client = spanner::Client(connection);
        auto rows = client.Read("RoutedSignal", spanner::KeySet::All(), 
            {"LifeCycleId", "CreateTime", "FromQueue", "ToQueue"});
        std::vector<RoutingSignal> list;
        for (auto const& row : rows) {
            if (!row) {
                std::cerr << row.status();
                break;
            }
            auto LifeCycleId = row->get<std::string>("LifeCycleId");
            auto CreateTime = row->get<google::cloud::spanner::v1::Timestamp>("CreateTime");
            auto FromQueue = row->get<std::string>("FromQueue");
            auto ToQueue = row->get<std::string>("ToQueue");

            RoutingSignal signal;
            signal.life_cycle_id = LifeCycleId.value();
            signal.create_time = CreateTime.value();
            signal.from_queue = FromQueue.value();
            signal.to_queue = ToQueue.value();
            list.push_back(signal);
        }
        return list;
    }
    std::vector<VerdictSignal> getAllVerdictSignals() {
        namespace spanner = ::google::cloud::spanner;
        auto database = spanner::Database(PROJECT_ID, INSTANCE_ID, DATABASE_ID);
        auto connection = spanner::MakeConnection(database);
        auto client = spanner::Client(connection);
        auto rows = client.Read("VerdictSignal", spanner::KeySet::All(), 
            {"LifeCycleId", "CreateTime", "QueueId", "SLA_min"});
        std::vector<VerdictSignal> list;
        for (auto const& row : rows) {
            if (!row) {
                std::cerr << row.status();
                break;
            }
            auto LifeCycleId = row->get<std::string>("LifeCycleId");
            auto CreateTime = row->get<google::cloud::spanner::v1::Timestamp>("CreateTime");
            auto QueueId = row->get<std::string>("QueueId");
            auto SLA_min = row->get<std::int64_t>("SLA_min");

            VerdictSignal signal;
            signal.life_cycle_id = LifeCycleId.value();
            signal.create_time = CreateTime.value();
            signal.queue_id = QueueId.value();
            signal.SLA_min = SLA_min.value();
            list.push_back(signal);
        }
        return list;
    }

    std::vector<Video> getAllVideos(){
        namespace spanner = ::google::cloud::spanner;
        auto database = spanner::Database(PROJECT_ID, INSTANCE_ID, DATABASE_ID);
        auto connection = spanner::MakeConnection(database);
        auto client = spanner::Client(connection);
        auto rows = client.Read("Videos", spanner::KeySet::All(), 
            {"Id", "Features"});
        std::vector<Video> list;
        for (auto const& row : rows) {
            if (!row) {
                std::cerr << row.status();
                break;
            }
            auto Id = row->get<std::string>("Id");
            auto Features = row->get<std::vector<std::string>>("Features");

            Video video;
            video.id = Id.value();
            video.features = Features.value();
            list.push_back(video);
        }
        return list;
    }

} // namespace youtube_hermes_config_subscriber