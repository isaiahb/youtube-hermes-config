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

	// // Open up the stream.
    ClientContext context;
    auto stream(stub->StreamingPull(&context));

    // // Send initial message.
    StreamingPullRequest request;
    request.set_subscription("projects/lab5-260516/subscriptions/testsub");
    request.set_stream_ack_deadline_seconds(10);
    stream->Write(request);

    // Receive messages.
    StreamingPullResponse response;
    while (stream->Read(&response)) {
      // Ack messages.
    	StreamingPullRequest ack_request;
    	for (const auto &message : response.received_messages()) {
    		ack_request.add_ack_ids(message.ack_id());
			std::cout<<"message"<<std::endl;
    	}
    	stream->Write(ack_request);
    }

	std::cout<<"wow"<<std::endl;
}



// //publish  example
// auto main() -> int {
//     using grpc::ClientContext;
//     using google::pubsub::v1::Publisher;
//     using google::pubsub::v1::PublishRequest;
//     using google::pubsub::v1::PublishResponse;
//     using google::pubsub::v1::PubsubMessage;

//     auto creds = grpc::GoogleDefaultCredentials();
// 	auto channel = grpc::CreateChannel("pubsub.googleapis.com", creds);
// 	std::unique_ptr<Publisher::Stub> stub(Publisher::NewStub(channel));

//     PublishRequest request;
//     auto const topic_string = "projects/lab5-260516/topics/testing";
//     request.set_topic(topic_string);
//     PubsubMessage msg;
//     msg.set_data("Hello from C++");
//     *request.add_messages() = msg;

//     PublishResponse response;
//     ClientContext ctx;

//     auto status = stub->Publish(&ctx, request, &response);
//     if (!status.ok()) {
//         std::cout << "Publishing message failed with error " + std::to_string(status.error_code()) + ": " + status.error_message() << '\n';
//     }
// }

//gcloud auth application-default login //<- cmd can be used to set GoogleDefaultCredentials
