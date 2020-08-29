"""This module holds the DatetimeUtils class which
provides utilities for generating and manipulating
datetime.datetime objects.
"""
import random
from datetime import datetime
from datetime import timedelta

class DatetimeUtils():
  """DatetimeUtils class
provides utilities for generating and manipulating
datetime.datetime objects.
  """

  def __init__(self, database):
    self.database = database

  def generate_random_datetime(self):
    """Generate a random UTC timestamp.

    Returns:
        datetime.datetime: a randomly generated datetime.datetime object
    """
    month = random.randint(1, 12)
    calendar_days = {
        1 : 31,
        2 : 28,
        3 : 31,
        4 : 30,
        5 : 31,
        6 : 30,
        7 : 31,
        8 : 31,
        9: 30,
        10 : 31,
        11 : 30,
        12 : 31
    }
    day = random.randint(1, calendar_days[month])
    hour = random.randint(0, 23)
    minute = random.randint(0, 59)
    second = random.randint(0, 59)
    date_time = datetime(2020, month, day, hour, minute, second, 0)
    return date_time

  def randomly_increment_datetime(self, date_time):
    """Increment a datetime.datetime object by a random number of minutes between
    1 and 59.

    Args:
        timestamp (datetime.datetime) : a datetime.datetime object

    Returns:
        datetime.datetime: the old datetime.datetime object combined with the random increment value
    """
    return date_time + timedelta(minutes=random.randint(1, 59))

  def convert_datetime_to_timestamp(self, date_time):
    """Converts a datetime.datetime object to a string in UTC Timestamp format.

    Args:
        date_time (datetime.datetime): a datetime.datetime object

    Returns:
        str: a UTC Timestamp string
    """
    date_time = str(date_time).split()
    return date_time[0] + "T" + date_time[1] + "Z"
