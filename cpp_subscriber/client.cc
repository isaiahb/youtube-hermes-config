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

#include "client.h"

#include <grpc++/grpc++.h>

#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#include "absl/strings/string_view.h"
#include "google/pubsub/v1/pubsub.grpc.pb.h"
#include "processor.h"

namespace youtube_hermes_config_subscriber {

std::string Client::get_pubsub_subscription_link() {
  return pubsub_subscription_link_;
}

// Any call to this method when the client is already running is ignored.
void Client::Run(MessageCallback callback) {
  if (!is_running_) {
    this->is_running_ = true;
    this->thread_ = std::make_unique<std::thread>(&Client::RunThreadFunction, this, callback);
  }
}

void Client::RunThreadFunction(MessageCallback callback) {
  using google::pubsub::v1::ReceivedMessage;
  using google::pubsub::v1::StreamingPullRequest;
  using google::pubsub::v1::StreamingPullResponse;
  using google::pubsub::v1::Subscriber;
  using grpc::ClientContext;

  std::cout << "Running client RunThreadFunction." << std::endl;

  auto credentials = grpc::GoogleDefaultCredentials();
  auto channel = grpc::CreateChannel("pubsub.googleapis.com", credentials);
  std::unique_ptr<Subscriber::Stub> stub(Subscriber::NewStub(channel));

  while (this->is_running_) {
    std::this_thread::sleep_for(std::chrono::seconds(10));

    // Open the stream.
    ClientContext context;
    std::unique_ptr<::grpc::ClientReaderWriterInterface<::google::pubsub::v1::StreamingPullRequest, ::google::pubsub::v1::StreamingPullResponse>> stream(stub->StreamingPull(&context));

    // Connect the stream to the Pub/Sub subscription.
    StreamingPullRequest request;
    request.set_subscription(this->pubsub_subscription_link_);
    request.set_stream_ack_deadline_seconds(10);

    // Poll for new messages.
    StreamingPullResponse response;
    stream->Write(request);

    while (stream->Read(&response)) {
      StreamingPullRequest ack_request;
      for (const ReceivedMessage& message : response.received_messages()) {
        // Acknowledge and process a new message, if there is data.
        ack_request.add_ack_ids(message.ack_id());
        if (message.has_message()) {
          callback(message.message());
        }
      }

      // Acknowledged messages.
      stream->Write(ack_request);
    }
  }  // While (is_running) loop.
  std::cout << "Stopping client RunThreadFunction." << std::endl;
}

void Client::Stop() {
  is_running_ = false;
}

void Client::JoinThread() {
  if (thread_) {
    thread_->join();
  }
}

bool Client::IsRunning() {
  return is_running_;
}

}  // namespace youtube_hermes_config_subscriber