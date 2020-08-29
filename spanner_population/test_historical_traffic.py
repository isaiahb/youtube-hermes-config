"""Test file for populating Spanner Historical Traffic database tables with dummy data."""
import unittest
import historical_traffic
import constants

class TestsHistoricalTraffic(unittest.TestCase):
  """Test methods from historical_traffic.py

  Args:
    unittest (unittest.TestCase): unittest Testcase
  """

  def test_get_videos_size(self):
    """Test get_table_size() for all tables. Assert that table is empty upon initialization.
    """
    h_t = historical_traffic.HistoricalTraffic()
    assert h_t.database.get_table_size("Videos") == 0 and \
      h_t.database.get_table_size("Queues") == 0 and \
        h_t.database.get_table_size("EnqueueRule") == 0 and \
          h_t.database.get_table_size("EnqueueSignal") == 0 and \
            h_t.database.get_table_size("RoutedSignal") == 0 and \
        h_t.database.get_table_size("VerdictSignal") == 0

  def test_populate_videos(self):
    """Test populate_videos() under normal conditions. Assert that size of Videos table is 3000.
    """
    h_t = historical_traffic.HistoricalTraffic()
    h_t.populate_videos()
    assert h_t.database.get_table_size("Videos") == constants.ROW_COUNT

  def test_populate_queues(self):
    """Test populate_queues() under normal conditions. Assert that size of Queues table is 3000.
    """
    h_t = historical_traffic.HistoricalTraffic()
    h_t.populate_queues()
    assert h_t.database.get_table_size("Queues") == constants.ROW_COUNT

  def test_populate_enqueue_rule(self):
    """Test populate_enqueue_rule() under normal conditions.
    Assert that size of EnqueueRule table is 3000.
    """
    h_t = historical_traffic.HistoricalTraffic()
    h_t.populate_queues()
    h_t.populate_enqueue_rule()
    assert h_t.database.get_table_size("EnqueueRule") == constants.ROW_COUNT

  def test_populate_video_lifecycle(self):
    """Test populate_video_lifecycle() under normal conditions.
    Assert that size of EnqueueSignal table is 3000, size of the RoutedSignal and Verdict Signal
    tables is smaller than or equal to the size of the EnqueueSignal table.
    """
    h_t = historical_traffic.HistoricalTraffic()
    h_t.populate_videos()
    h_t.populate_queues()
    h_t.populate_video_lifecycle()

    enqueue_signal_table_size = h_t.database.get_table_size("EnqueueSignal")
    routed_signal_table_size = h_t.database.get_table_size("RoutedSignal")
    verdict_signal_table_size = h_t.database.get_table_size("VerdictSignal")

    assert enqueue_signal_table_size == constants.ROW_COUNT and routed_signal_table_size <= \
    enqueue_signal_table_size and verdict_signal_table_size <= enqueue_signal_table_size

  def test_valid_lifecycle_timestamp(self):
    """Test that EnqueueSignal timestamps are created before corresponding RoutedSignals
    and RoutedSignals are created before respective VerdictSignals.


    Note : In reality, an entity could be routed multiple times, however, for simplicty,
    we are assuming entities can only be routed ONCE at most.
    """
    h_t = historical_traffic.HistoricalTraffic()
    h_t.populate_videos()
    h_t.populate_queues()
    h_t.populate_video_lifecycle()

    #Checks that EnqueueSignal timestamps are created before corresponding RoutedSignals
    #and RoutedSignals are created before respective VerdictSignals.
    query = "SELECT count(*) FROM EnqueueSignal "\
      "FULL OUTER JOIN RoutedSignal ON EnqueueSignal.LifeCycleId = RoutedSignal.LifeCycleId "\
      "FULL OUTER JOIN VerdictSignal ON EnqueueSignal.LifeCycleId = VerdictSignal.LifeCycleId "\
      "WHERE EnqueueSignal.CreateTime > RoutedSignal.CreateTime "\
      "OR RoutedSignal.CreateTime > VerdictSignal.CreateTime "\
      "OR EnqueueSignal.CreateTime > VerdictSignal.CreateTime"

    result = h_t.database.execute_query(query)
    assert result == 0

  def test_valid_lifecycle_queue_id(self):
    """Test that RoutedSignal and VerdictSignal Queue Ids match a respective EnqueueSignal Queue Id
    for a LifeCycleId.

     Note: In reality, an entity could be routed multiple times,
    however, for simplicty, we are assuming entities can only be routed ONCE at most.
    """
    h_t = historical_traffic.HistoricalTraffic()
    h_t.populate_videos()
    h_t.populate_queues()
    h_t.populate_enqueue_rule()
    h_t.populate_video_lifecycle()

    #Number of entries where the EnqueueSignal.QueueMatch = RoutedSignal.FromQueue
    query = "SELECT count(*) "\
      "FROM EnqueueSignal FULL OUTER JOIN RoutedSignal "\
      "ON EnqueueSignal.LifeCycleId = RoutedSignal.LifeCycleId "\
      "WHERE EnqueueSignal.QueueMatch = RoutedSignal.FromQueue"

    routed_signal_queue_id_matches = h_t.database.execute_query(query)

    #Number of entries where the EnqueueSignal.QueueMatch = VerdictSignal.QueueId or
    #RoutedSignal.ToQueue = VerdictSignal.QueueId
    query = "SELECT count(*) "\
      "FROM EnqueueSignal FULL OUTER JOIN RoutedSignal "\
      "ON EnqueueSignal.LifeCycleId = RoutedSignal.LifeCycleId "\
      "FULL OUTER JOIN VerdictSignal ON EnqueueSignal.LifeCycleId = VerdictSignal.LifeCycleId "\
      "WHERE EnqueueSignal.QueueMatch = VerdictSignal.QueueId "\
        "OR RoutedSignal.ToQueue = VerdictSignal.QueueId"

    verdict_signal_queue_id_matches = h_t.database.execute_query(query)

    assert routed_signal_queue_id_matches == h_t.database.get_table_size("RoutedSignal") and \
    verdict_signal_queue_id_matches == h_t.database.get_table_size("VerdictSignal")

if __name__ == '__main__':
  unittest.main()
