"""This module holds the HistoricalTraffic class which populates
the spanner database with dummy data.
"""
import random
import constants
import datetime_utils
import database

class HistoricalTraffic():
  """Contains tools to clear and populate the historical traffic spanner database
  with dummy data for Hermes Buganizer CR Automation.
  """
  def __init__(self):
    self.database = database.Database()
    self.datetime_utils = datetime_utils.DatetimeUtils(self.database)
    self.queue_routes = {}
    self.clear_all_tables()

  def populate_videos(self):
    """Populate the Videos table with dummy data.
    """
    table_values = []
    for i in range(constants.ROW_COUNT):
      video_id = str(i)

      features = random.sample(range(constants.FEATURES_COUNT),
                               random.randint(0, constants.MAX_FEATURES_PER_VIDEO))
      for j, item in enumerate(features):
        features[j] = "Feature - " + str(item)

      table_values.append((video_id, features))

    table_name = 'Videos'
    columns = ('Id', 'Features')

    self.database.table_insert(table_name, columns, table_values)

  def populate_queues(self):
    """Populate the Queues table with dummy data.
    """
    table_values = []
    for i in range(constants.ROW_COUNT):
      queue_id = str(i)
      desired_sla_minutes = random.randint(0, constants.DESIRED_SLA_COUNT)

      owners = ["owner" + str(i) + "@google.com"]

      possible_routes = random.sample(range(constants.ROW_COUNT),
                                      random.randint(0, constants.MAX_POSSIBLE_ROUTES))
      for j, item in enumerate(possible_routes):
        possible_routes[j] = str(item)

      self.queue_routes[queue_id] = possible_routes

      queue_name = "Queue - " + str(i)
      table_values.append((queue_id, desired_sla_minutes, owners, possible_routes, queue_name))

    table_name = 'Queues'
    columns = ('Id', 'DesiredSLA_min', 'Owners', 'PossibleRoutes', 'QueueName')

    self.database.table_insert(table_name, columns, table_values)

  def populate_enqueue_rule(self):
    """Populate the EnqueueRule table with dummy data.
    """
    table_values = []
    for i in range(constants.ROW_COUNT):
      enqueue_rule_id = str(i)
      priority = i
      queue_id = str(random.randint(0, constants.ROW_COUNT - 1))

      rules = random.sample(range(constants.RULE_COUNT),
                            random.randint(0, constants.MAX_RULES_PER_ENQUEUE_RULES))
      for j, item in enumerate(rules):
        rules[j] = "Feature - " + str(item)

      table_values.append((enqueue_rule_id, priority, queue_id, rules))

    table_name = 'EnqueueRule'
    columns = ('Id', 'Priority', 'QueueId', 'Rule')

    self.database.table_insert(table_name, columns, table_values)


  def populate_video_lifecycle(self):
    """Populate the EnqueueSignal, RoutedSignal, and VerdictSignal tables with sample data
    that covers all cases of a lifecycle. In reality, an entity could be routed multiple times,
    however, for simplicty, we are assuming entities can only be routed ONCE at most.
    """
    lifecycle_dict = {}
    lifecycle_dict = self.populate_enqueue_signal(lifecycle_dict)
    lifecycle_dict = self.populate_routed_signal(lifecycle_dict)
    self.populate_verdict_signal(lifecycle_dict)

  def populate_enqueue_signal(self, lifecycle_dict):
    """Populate the EnqueueSignal table with dummy data.

    Args:
      lifecycle_dict(dict) : a dictionary mapping LifeCycleIds to a tuple containing
      a queue id in index 0 and a datetime.datetime object in index 1.
    """
    table_values = []
    for i in range(constants.ROW_COUNT):
      life_cycle_id = str(i)
      video_id = str(random.randint(0, constants.ROW_COUNT - 1))
      queue_match = str(i)
      violation_category = "Category - " + str(i)

      date_time = self.datetime_utils.generate_random_datetime()

      #Update lifecycle_dict with the new values
      lifecycle_dict[life_cycle_id] = (queue_match, date_time)

      create_time = self.datetime_utils.convert_datetime_to_timestamp(date_time)

      table_values.append((life_cycle_id, video_id, queue_match, violation_category, create_time))

    table_name = 'EnqueueSignal'
    columns = ('LifeCycleId', 'VideoId', 'QueueMatch', 'ViolationCategory', 'CreateTime')

    self.database.table_insert(table_name, columns, table_values)
    return lifecycle_dict

  def populate_routed_signal(self, lifecycle_dict):
    """Populate the RoutedSignal table with dummy data.

    Args:
      lifecycle_dict(dict) : a dictionary mapping LifeCycleIds to a tuple containing
      a queue id in index 0 and a datetime.datetime object in index 1.

    Returns:
      dict : an updated dictionary mapping LifeCycleIds to a tuple containing
      a queue id in index 0 and a datetime.datetime object in index 1.
    """
    table_values = []
    for i in range(constants.ROW_COUNT):
      life_cycle_id = str(i)

      #See where the item previously was
      from_queue = lifecycle_dict[life_cycle_id][0]

      #Select a random queue from possible routes if possible
      to_queue_list = self.queue_routes[from_queue]

      if len(to_queue_list) == 0:
        continue

      to_queue = random.choice(to_queue_list)

      #Increment the previous datetime by a random value
      date_time = self.datetime_utils.randomly_increment_datetime(lifecycle_dict[life_cycle_id][1])

      table_values.append([life_cycle_id, to_queue, from_queue, date_time])

    table_name = 'RoutedSignal'
    columns = ('LifeCycleId', 'ToQueue', 'FromQueue', 'CreateTime')

    random.shuffle(table_values)

    top_k = random.randint(0, constants.ROW_COUNT)

    #Update lifecycle dict with the top k values
    for i in range(top_k):
      row = table_values[i]
      lifecycle_dict[row[0]] = (row[1], row[3])
      row[3] = self.datetime_utils.convert_datetime_to_timestamp(row[3])

    #Fill the database with the top k values
    self.datetime_utils.database.table_insert(table_name, columns, table_values[:top_k])
    return lifecycle_dict

  def populate_verdict_signal(self, lifecycle_dict):
    """Populate the VerdictSignal table with dummy data.

    Args:
      lifecycle_dict(dict) : a dictionary mapping LifeCycleIds to a tuple containing
      a queue id in index 0 and a datetime.datetime object in index 1.

    Returns:
      dict : an updated dictionary mapping LifeCycleIds to a tuple containing
      a queue id in index 0 and a datetime.datetime object in index 1.
    """
    table_values = []
    for i in range(constants.ROW_COUNT):
      life_cycle_id = str(i)
      queue_id = lifecycle_dict[life_cycle_id][0]

      date_time = self.datetime_utils.randomly_increment_datetime(lifecycle_dict[life_cycle_id][1])
      create_time = self.datetime_utils.convert_datetime_to_timestamp(date_time)
      sla_minutes = random.randint(0, constants.MAX_SLA_MINUTES)

      table_values.append((life_cycle_id, create_time, queue_id, sla_minutes))

      #No need to update lifecycle_dict because this is the end of the lifecycle

    table_name = 'VerdictSignal'
    columns = ('LifeCycleId', 'CreateTime', 'QueueId', 'SLA_min')

    random.shuffle(table_values)

    top_k = random.randint(0, constants.ROW_COUNT)

    #Fill the database with the top k values
    self.database.table_insert(table_name, columns, table_values[:top_k])

  def clear_all_tables(self):
    """Clear all tables in the historical traffic database.
    """
    self.database.delete_all_from_table("EnqueueRule")
    self.database.delete_all_from_table("EnqueueSignal")
    self.database.delete_all_from_table("RoutedSignal")
    self.database.delete_all_from_table("VerdictSignal")
    self.database.delete_all_from_table("Queues")
    self.database.delete_all_from_table("Videos")

if __name__ == '__main__':
  historical_traffic = HistoricalTraffic()

  historical_traffic.populate_videos()
  historical_traffic.populate_queues()
  historical_traffic.populate_enqueue_rule()
  historical_traffic.populate_video_lifecycle()
