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
	
	//keep alive for duration of machines life
	while (true) {
		std::this_thread::sleep_for(std::chrono::seconds(10));

		// Open stream.
		ClientContext context;
		auto stream(stub->StreamingPull(&context));
		// connect stream to pubsub subscription
		StreamingPullRequest request;
		request.set_subscription("projects/google.com:youtube-admin-pacing-server/subscriptions/CppBinary");
		request.set_stream_ack_deadline_seconds(10);

		// poll for messages.
		StreamingPullResponse response;
		stream->Write(request);

		while (stream->Read(&response)) {
			StreamingPullRequest ack_request;
			for (const auto &message : response.received_messages()) {
				ack_request.add_ack_ids(message.ack_id());
				bool hasMessage = message.has_message();
				if (hasMessage) {
					Process(message.message());
				}
			}
			
			// acknowledged messages.
			stream->Write(ack_request);
		}
	}
	std::cout<<"program ending"<<std::endl;
}

void Process(google::pubsub::v1::PubsubMessage message) {
	using google::protobuf::Map;
	using google::pubsub::v1::PubsubMessage;
	using std::string;
	using std::cout;
	using std::endl;

	string data = message.data();
	Map<string, string> attributes = message.attributes();
	cout<<"data: "<<data<<endl<<endl;


	switch(data) {
		case "EnqueueRule" :
			string features = attributes["Features"];
			string queue = attributes["Queue"];
			cout << "EnqueueRule: " << features << " -> " << queue << endl;
			break;
		case "RoutingRule" :
			string possibleRoutes = attributes["PossibleRoutes"];
			string queue = attributes["Queue"];
			cout << "RoutingRule: " << queue << " -> " << possibleRoutes << endl;
			break;
		case "QueueInfo" :
			string owners = attributes["Owners"];
			cout << "Queue-Info: Owners: " << owners << endl;
			break;
		default :
		cout << "Invalid Configuration. The value of message's data must be one of the following (EnqueueRule, RoutingRule, QueueInfo)" << endl;
	}
}