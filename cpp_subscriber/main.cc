#include <iostream>
#include <memory>
#include <grpc++/grpc++.h>
#include "google/pubsub/v1/pubsub.grpc.pb.h"
#include<thread>
#include<chrono>

void Process(std::string);

int main() {
	using grpc::ClientContext;
	using google::pubsub::v1::Subscriber;
	using google::pubsub::v1::PullRequest;
	using google::pubsub::v1::PullResponse;
	using google::pubsub::v1::StreamingPullRequest;
	using google::pubsub::v1::StreamingPullResponse;
	using google::pubsub::v1::PubsubMessage;

	auto credentials = grpc::GoogleDefaultCredentials();	
	auto channel = grpc::CreateChannel("pubsub.googleapis.com", credentials);
	std::unique_ptr<Subscriber::Stub> stub(Subscriber::NewStub(channel));

	// Open stream.
	ClientContext context;
	auto stream(stub->StreamingPull(&context));
	// connect stream to pubsub subscription
	StreamingPullRequest request;
	request.set_subscription("projects/google.com:youtube-admin-pacing-server/subscriptions/CppBinary");
	request.set_stream_ack_deadline_seconds(10);
	stream->Write(request);

	// poll for messages.
	int i = 0;
	StreamingPullResponse response;

	//keep alive for duration of machines life
	while (true) {
		while (stream->Read(&response)) {
			i++;
			std::cout<<"Reading: "<<i<<std::endl;
			StreamingPullRequest ack_request;
			for (const auto &message : response.received_messages()) {
				ack_request.add_ack_ids(message.ack_id());
				bool hasMessage = message.has_message();
				if (hasMessage) {
					Process(message.message().data());
				}
			}
			
			// acknowledged messages.
			stream->Write(ack_request);
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	std::cout<<"program finished"<<std::endl;
}


void Process(std::string message) {
	std::cout<<"message received: "<<message<<std::endl;
}
