"""This module is responsible for creating and publishing Protocol Buffer Objects via Pub/Sub
messages according to the configuration change request type specified in the template
submitted by the reporter of the Buganizer issue. It holds the Context, ConfigurationTypes,
EnqueueRules, RoutingTargets, and QueueInfo classes and uses the factory design pattern.
"""
from google.cloud import pubsub_v1
import constants
from config_change_request import config_change_pb2

class ConfigurationTypeFactory():
  """The Facory for making the different types of configurations
  (EnqueueRules, RoutingTargets, QueueInfo)
  """

  def __init__(self, advanced_fields_dict=dict()):
    """Initialize the ConfigurationTypeFactory

    Args:
        advanced_fields (dict): dictionary that holds all advanced fields values
    """
    if not advanced_fields_dict:
      self.config_change_type = "EnqueueRules"
    else:
      self.advanced_fields_dict = advanced_fields_dict
      self.config_change_type = advanced_fields_dict["Config Type"]

    self.config_change_request = config_change_pb2.ConfigChangeRequest()

  def make(self):
    """Make a ConfigChangeRequest object according to the
    config change type.

    Returns:
    ConfigChangeRequest: The config change request object containing either the complete
      Proto Buf object or no Proto Buf object and a detailed error message for logging.
    """
    if self.config_change_type == "RoutingTargets":
      return self.make_routing_targets()
    elif self.config_change_type == "QueueInfo":
      return self.make_queue_info()

  def make_enqueue_rules(self, template, reporter, comment, issue):
    """Parses the template into a ConfigChangeRequest object that will
     be ready to send, given it is valid.

    Args:
      template (list): a list of lines from the reporter's comment
      reporter (str) : the reporter of the Buganizer issue
      comment (str) : the reporters comment of the configuration template
      issue (str) : the URL of the Buganizer issue

    Returns:
      ConfigChangeRequest: The config change request object containing either the complete
      Proto Buf object or no Proto Buf object and a detailed error message for logging.
    """
    self.config_change_request.issue_id = issue.replace("https://b.corp.google.com/issues/", "")
    config_change_request = ConfigurationChangeRequest()
    #These keep track of the current line number of each specifier for EnqueueRule.
    #Starts at 1 since the first line is reserved for the Configuration specifier.
    method_specifier_line_number = 1
    queue_specifier_line_number = 2
    features_specifier_line_number = 3
    priority_specifier_line_number = 4

    #Length of each specifier
    method_specifier_length = len(constants.METHOD_SPECIFIER)
    queue_specifier_length = len(constants.QUEUE_SPECIFIER)
    features_specifier_length = len(constants.ENQUEUE_RULE_FEATURES_SPECIFIER)
    priority_specifier_length = len(constants.ENQUEUE_RULE_PRIORITY_SPECIFIER)

    #This checks that the template is the correct number of lines long
    if (len(template) - 1) % constants.ENQUEUE_RULE_COMMAND_LINES_COUNT > 0:
      error_message = "The following configuration change request from " + \
      issue + " is invalid. The number of lines does not match for a correct EnqueueRule "\
        "template. Please check that the format correctly matches template and try again.\n" + \
      comment + "\n"
      config_change_request.error_message = error_message
      return config_change_request

    for line_idx in range(1, len(template), constants.ENQUEUE_RULE_COMMAND_LINES_COUNT):

      method_specifier_line_number, queue_specifier_line_number, features_specifier_line_number, \
             priority_specifier_line_number = (
                 range(line_idx, line_idx + constants.ENQUEUE_RULE_COMMAND_LINES_COUNT))

      change = self.config_change_request.enqueue_rules.Change()
      change.reporter = reporter

      method_specifier = template[method_specifier_line_number][:method_specifier_length]
      queue_specifier = template[queue_specifier_line_number][:queue_specifier_length]
      features_specifier = template[features_specifier_line_number][:features_specifier_length]
      priority_specifier = template[priority_specifier_line_number][:priority_specifier_length]

      if method_specifier != constants.METHOD_SPECIFIER or queue_specifier != \
        constants.QUEUE_SPECIFIER or features_specifier != \
          constants.ENQUEUE_RULE_FEATURES_SPECIFIER or priority_specifier != \
            constants.ENQUEUE_RULE_PRIORITY_SPECIFIER:
        error_message = "The following configuration change request from " + \
        issue + " is invalid. One or more specifiers are not correct for a EnqueueRule "\
        "template. Please check that the format correctly matches template and try again.\n" + \
        comment + "\n"
        config_change_request.error_message = error_message
        return config_change_request

      change.method = template[method_specifier_line_number][method_specifier_length:]

      if change.method not in constants.POSSIBLE_METHODS:
        error_message = "The following configuration change request from " + \
        issue + " is invalid. The method entry '" + change.method + "' is not a valid method."\
        "Please check that the method is 'Add', 'Remove', or 'Set' and try again.\n" + \
        comment + "\n"
        config_change_request.error_message = error_message
        return config_change_request

      change.queue = template[queue_specifier_line_number][queue_specifier_length:]
      change.features.extend(template[features_specifier_line_number][features_specifier_length:].\
        split(", "))
      change.priority = int(template[priority_specifier_line_number][priority_specifier_length:])
      self.config_change_request.enqueue_rules.changes.append(change)

    config_change_request.proto = self.config_change_request

    return config_change_request


  def make_routing_targets(self):
    """Parses the template into a ConfigChangeRequest object that will be ready to send,
    given it is valid.

    Returns:
      ConfigChangeRequest: The config change request object containing either the complete
      Proto Buf object or no Proto Buf object and a detailed error message for logging.
    """
    config_change_request = ConfigurationChangeRequest()

    advanced_fields = self.config_change_request.routing_targets

    for field_name, field_value in self.advanced_fields_dict.items():
      try:
        if field_name == "Issue Id":
          self.config_change_request.issue_id = field_value
        elif field_name == "Config Type":
          self.config_change_request.config_type = field_value
        elif field_name == "Severity":
          advanced_fields.severity = advanced_fields.Severity.Value(field_value)
        elif field_name == "Found In":
          values = field_value.replace(" ", "").split(",")
          advanced_fields.found_in.extend(values)
        elif field_name == "In Prod":
          advanced_fields.in_prod = field_value
        elif field_name == "Reporter":
          advanced_fields.reporter = field_value
        elif field_name == "Verifier":
          advanced_fields.verifier = field_value
        elif field_name == "Targeted To":
          values = field_value.replace(" ", "").split(",")
          advanced_fields.targeted_to.extend(values)
        elif field_name == "Queue Id":
          advanced_fields.queue_id = field_value
        elif field_name == "Add Queues to Route To":
          advanced_fields.add_queues_to_route_to.extend(field_value)
        elif field_name == "Remove Queues to Route To":
          advanced_fields.remove_queues_to_route_to.extend(field_value)
      except AttributeError:
        error_message = "The config_change.proto file may not be compiled. See the "\
          "README.md for insructions"
        config_change_request.error_message = error_message
        return config_change_request

    config_change_request.proto = self.config_change_request

    return config_change_request

  def make_queue_info(self):
    """Parses the template into a ConfigChangeRequest object that will
     be ready to send, given it is valid.

    Returns:
      ConfigChangeRequest: The config change request object containing either the complete
      Proto Buf object or no Proto Buf object and a detailed error message for logging.
    """
    config_change_request = ConfigurationChangeRequest()
    advanced_fields = self.config_change_request.queue_info

    for field_name, field_value in self.advanced_fields_dict.items():
      try:
        if field_name == "Issue Id":
            self.config_change_request.issue_id = field_value
        elif field_name == "Config Type":
          self.config_change_request.config_type = field_value
        elif field_name == "Severity":
          advanced_fields.severity = advanced_fields.Severity.Value(field_value)
        elif field_name == "Found In":
          values = field_value.replace(" ", "").split(",")
          advanced_fields.found_in.extend(values)
        elif field_name == "In Prod":
          advanced_fields.in_prod = field_value
        elif field_name == "Reporter":
          advanced_fields.reporter = field_value
        elif field_name == "Verifier":
          advanced_fields.verifier = field_value
        elif field_name == "Targeted To":
          values = field_value.replace(" ", "").split(",")
          advanced_fields.targeted_to.extend(values)
        elif field_name == "Queue Id":
          advanced_fields.queue_id = field_value
        elif field_name == "MDB group name":
          advanced_fields.mdb_group_name = field_value
        elif field_name == "Ops Owner":
          advanced_fields.ops_owner = field_value
        elif field_name == "GVO Owner":
          advanced_fields.gvo_owner = field_value
        elif field_name == "Tech Owner":
          advanced_fields.tech_owner = field_value
        elif field_name == "Is Dashboard Queue":
          advanced_fields.is_dashboard_queue = field_value
        elif field_name == "Reviews per Item":
          advanced_fields.reviews_per_item = field_value
        elif field_name == "Fragment Name":
          advanced_fields.fragment_name = field_value
        elif field_name == "Item Expiry (Sec)":
          advanced_fields.item_expiry_sec = field_value
        elif field_name == "Is Experimental Review Enabled":
          advanced_fields.is_experimental_review_enabled = field_value
        elif field_name == "Experimental Probability":
          advanced_fields.experimental_probability = field_value
      except AttributeError:
        error_message = "The config_change.proto file may not be compiled. See the "\
          "README.md for insructions"
        config_change_request.error_message = error_message
        return config_change_request

    config_change_request.proto = self.config_change_request

    return config_change_request

  def publish(self, config_change_request):
    """Publishes a message to a Pub/Sub topic.
    """
    client = pubsub_v1.PublisherClient()
    topic_path = client.topic_path(constants.PROJECT_ID, constants.TOPIC_NAME)
    data = config_change_request.SerializeToString()
    client.publish(topic_path, data=data)

class ConfigurationChangeRequest():
  """A wrapper object to hold configuration change request Proto Buf objects
  or a error message if neccessary.
  """
  def __init__(self):
    self.proto = None
    self.error_message = ""
