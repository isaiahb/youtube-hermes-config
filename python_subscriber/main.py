"""This module holds the System class which contains the Pub/Sub subscriber
client and completes any necessary setup for scraping.
"""
import os
from concurrent import futures
from google.cloud import pubsub_v1
from google.protobuf.message import DecodeError
import web_scraping_utility
import impact_analysis_response_pb2
from graph_response import graph_analysis
import constants
import logs.logger

class System():
  """Completes necessary setup for scraping."""
  def __init__(self):
    logger = logs.logger.Logger()
    self.web_scraping_util = web_scraping_utility.WebScrapingUtility(logger)
    self.set = set()


  def subscribe(self, project_id, subscription_id):
    """Creates a subscriber client and pulls Impact Analysis Response
    protobuf objects from a Pub/Sub topic.
    """
    subscriber = pubsub_v1.SubscriberClient()
    subscription_path = subscriber.subscription_path(project_id, subscription_id)

    def callback(message):
      """Processes PubSub message and deal with it accordingly.

      Args:
          message (serialized proto): the serialized impact analysis protobuf
      """
      impact_analysis_response = impact_analysis_response_pb2.ImpactAnalysisResponse()

      try:
        impact_analysis_response.ParseFromString(message.data)
      except DecodeError: #Pub Sub message couldn't be deserialized
        error_message = "Decode Error: Unable to parse pubsub message: " + message.data
        self.web_scraping_util.logger.log(error_message)

      issue = "https://b.corp.google.com/issues/" + impact_analysis_response.request.issue_id
      self.web_scraping_util.get_issue(issue)
      status = self.web_scraping_util.get_issue_status()
      assignee = self.web_scraping_util.get_issue_assignee()

      if status == "Fixed" or assignee != constants.AUTOMATION_USER:
        message.ack()
        return
      elif len(impact_analysis_response.queue_Impact_analysis_list) != 0 \
      and len(impact_analysis_response.error_message) == 0:
        grapher = graph_analysis.GraphAnalysis()
        png_path = grapher.graph_impact(impact_analysis_response)
        self.web_scraping_util.post_image(png_path)
        self.web_scraping_util.mark_as_fixed()
        os.remove(png_path)
      elif len(impact_analysis_response.queue_Impact_analysis_list) == 0:
        error_message = "There is no impact analysis for this change request. " + \
        impact_analysis_response.error_message
        self.web_scraping_util.logger.log(error_message)
        self.web_scraping_util.print_error_to_buganizer(error_message)

        self.web_scraping_util.mark_as_fixed()
      else:
        error_message = "There is no impact analysis for this change request. " + \
        impact_analysis_response.error_message + "Please re-assign the issue Assignee "\
          "value to the automation user '" + constants.AUTOMATION_USER + \
            "' when you are finished editing the issue."
        self.web_scraping_util.logger.log(error_message)
        self.web_scraping_util.print_error_to_buganizer(error_message)

        if impact_analysis_response.request.config_type == "EnqueueRules":
          self.web_scraping_util.set_assignee_to_reporter(
              impact_analysis_response.request.enqueue_rules.changes[0].reporter)
        elif impact_analysis_response.request.config_type == "RoutingTargets":
          self.web_scraping_util.set_assignee_to_reporter(
              impact_analysis_response.request.routing_targets.reporter)
        else:
          self.web_scraping_util.set_assignee_to_reporter(
              impact_analysis_response.request.queue_info.reporter)
      message.ack()

    #Limit the amount of messages processed in the subscriber callback function to one
    executor = futures.ThreadPoolExecutor(max_workers=1)
    scheduler = pubsub_v1.subscriber.scheduler.ThreadScheduler(executor)
    future = subscriber.subscribe(subscription_path, callback=callback,
                                  scheduler=scheduler)

    with subscriber:
      try:
        future.result()
      except TimeoutError:
        future.cancel()
    return

if __name__ == '__main__':
  system = System()
  system.subscribe(constants.PROJECT_ID, constants.SUBSCRIPTION_ID)
