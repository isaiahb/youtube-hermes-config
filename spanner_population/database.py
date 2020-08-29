"""This module holds the Database class which holds all google.cloud.spanner
  logic for easy access to the database from other modules.
"""
from google.cloud import spanner
import constants

class Database():
  """Holds all google.cloud.spanner
  logic for easy access to the database from other modules.
  """
  def __init__(self):
    self.database = self.create_database()

  def create_database(self):
    """Create a spanner database client and instance.

    Returns:
        spanner_v1.database.Database: the spanner database instance
    """
    spanner_client = spanner.Client()
    instance = spanner_client.instance(constants.INSTANCE_ID)
    return instance.database(constants.DATABASE_ID)


  def delete_all_from_table(self, table_name):
    """Delete all values from a given table.

    Args:
        table_name (str): the name of the table to delete from
    """
    with self.database.batch() as batch:
      remaining_values = spanner.KeySet(all_=True)
      batch.delete(table_name, remaining_values)

  def get_table_size(self, table_name):
    """Queries the number of rows from a table in the database using SQL.

    Args:
        table_name (str): The name of the database table
    Returns:
        int: the number of rows in the table
    """
    switcher = {
        "Videos": "SELECT count(*) FROM Videos",
        "Queues": "SELECT count(*) FROM Queues",
        "EnqueueRule": "SELECT count(*) FROM EnqueueRule",
        "EnqueueSignal": "SELECT count(*) FROM EnqueueSignal",
        "VerdictSignal": "SELECT count(*) FROM VerdictSignal",
        "RoutedSignal": "SELECT count(*) FROM RoutedSignal"
    }

    if not switcher.get(table_name):
      return

    with self.database.snapshot() as snapshot:
      results = snapshot.execute_sql(switcher.get(table_name))
      for row in results:
        return row[0]

  def execute_query(self, query):
    """Executes a given SQL query.

    Args:
        query (str): the SQL query to execute
    Returns:
        int: the number of rows in the table
    """

    with self.database.snapshot() as snapshot:
      results = snapshot.execute_sql(query)
      for row in results:
        return row[0]

  def table_insert(self, table_name, columns, values):
    """Insert values into a existing table in the database.

    Args:
        table_name (str): The name of the database table
        columns (str tuple): A tuple of strings representing the table column names
        values (list): a list of values to be inserted cooresponding to the columns
    """
    with self.database.batch() as batch:
      batch.insert(
          table=table_name,
          columns=columns,
          values=values)
