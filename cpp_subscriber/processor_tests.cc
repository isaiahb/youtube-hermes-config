#include <google/protobuf/util/message_differencer.h>

#include <vector>

#include "proto/config_change.pb.h"
#include "gtest/gtest.h"
#include "mock_message.h"
#include "processor.h"

TEST(MessageProcessor, EnqueueRuleTest) {
  using google::protobuf::util::MessageDifferencer;
  using google::protobuf::util::Status;
  using google::protobuf::util::StatusOr;
  using youtube_hermes_config_subscriber::MessageProcessor;
  using youtube_hermes_config_subscriber::MockMessage;

  // Create a ConfigChangeRequest object with an EnqueueRule change for MockMessage to serialize to string.
  ConfigChangeRequest config;
  EnqueueRule_Change* change = config.mutable_enqueue_rule()->add_changes();
  change->set_method("Add");
  change->set_queue("q1");
  change->add_features("f1");
  change->add_features("f2");
  change->add_features("f3");
  change->set_priority(1);

  // Create a MockMessage and obtain the deserialized config request from MessageProcessor function.
  MockMessage message = MockMessage(config);
  StatusOr<ConfigChangeRequest> request = MessageProcessor<MockMessage>(message);
  ConfigChangeRequest config_change_request = request.ValueOrDie();

  // Check that the MockMessages match.
  EXPECT_EQ(MessageDifferencer::Equals(config_change_request, config), true);
}

TEST(MessageProcessor, RoutingRuleTest) {
  using google::protobuf::util::MessageDifferencer;
  using google::protobuf::util::Status;
  using google::protobuf::util::StatusOr;
  using youtube_hermes_config_subscriber::MessageProcessor;
  using youtube_hermes_config_subscriber::MockMessage;

  // Create ConfigChangeRequest object with a RoutingRule change for MockMessage to serialize to string.
  ConfigChangeRequest config;
  RoutingRule_Change* change = config.mutable_routing_rule()->add_changes();
  change->set_method("Add");
  change->set_queue("q1");
  change->add_possible_routes("q2");
  change->add_possible_routes("q3");
  change->add_possible_routes("q4");

  // Create MockMessage and obtain deserialized config request from MessageProcessor function.
  MockMessage message = MockMessage(config);
  StatusOr<ConfigChangeRequest> request = MessageProcessor<MockMessage>(message);
  ConfigChangeRequest config_change_request = request.ValueOrDie();

  // Check that the MockMessages match.
  EXPECT_EQ(MessageDifferencer::Equals(config_change_request, config), true);
}

TEST(MessageProcessor, QueueInfoTest) {
  using google::protobuf::util::MessageDifferencer;
  using google::protobuf::util::Status;
  using google::protobuf::util::StatusOr;
  using youtube_hermes_config_subscriber::MessageProcessor;
  using youtube_hermes_config_subscriber::MockMessage;

  // Create ConfigChangeRequest object with a QueueInfo change for MockMessage to serialize to string.
  ConfigChangeRequest config;
  QueueInfo_Change* change = config.mutable_queue_info()->add_changes();
  change->set_method("Add");
  change->set_queue("q1");
  change->add_owner("isaiah");
  change->add_owner("saulo");

  // Create MockMessage and obtain deserialized config request from MessageProcessor function.
  MockMessage message = MockMessage(config);
  StatusOr<ConfigChangeRequest> request = MessageProcessor<MockMessage>(message);
  ConfigChangeRequest config_change_request = request.ValueOrDie();

  // Check that the MockMessages match.
  EXPECT_EQ(MessageDifferencer::Equals(config_change_request, config), true);
}

TEST(MessageProcessor, InvalidTest) {
  using google::protobuf::util::Status;
  using google::protobuf::util::StatusOr;
  using youtube_hermes_config_subscriber::MessageProcessor;
  using youtube_hermes_config_subscriber::MockMessage;

  // Create a ConfigChangeRequest object with a Empty/Invalid change for MockMessage to serialize to string.
  ConfigChangeRequest config;

  // Create MockMessage and obtain deserialized config request from MessageProcessor function.
  MockMessage message = MockMessage(config);
  StatusOr<ConfigChangeRequest> config_change_request = MessageProcessor<MockMessage>(message);

  // Check that the status of the config change request is ok.
  EXPECT_FALSE(config_change_request.ok());
}