#include <iostream>
#include <memory>
#include <grpc++/grpc++.h>
#include "google/pubsub/v1/pubsub.grpc.pb.h"

int main() {
    using grpc::ClientContext;
    using google::pubsub::v1::Subscriber;
    using google::pubsub::v1::PullRequest;
    using google::pubsub::v1::PullResponse;
	using google::pubsub::v1::StreamingPullRequest;
	using google::pubsub::v1::StreamingPullResponse;

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
    StreamingPullResponse response;
    while (stream->Read(&response)) {
      // acknowledged messages.
    	StreamingPullRequest ack_request;
    	for (const auto &message : response.received_messages()) {
    		ack_request.add_ack_ids(message.ack_id());
			std::cout<<"message received and acknowledged"<<std::endl;
           //std::cout<<message<<std::endl; //figure out how to properly get message text
    	}
    	stream->Write(ack_request);
    }

	std::cout<<"program finished"<<std::endl;
}