"""This module creates a logger that can be used by any module to log information
for the user. A new logger is created each time the program is run using a timestamp
to ensure a unique name.
"""
import logging
from datetime import datetime

class Logger():
  """Creates a timestamped log file in the logs/ directory
  and prints the systems errors in the log.
  """

  def __init__(self):
    self.logger = logging.getLogger()
    self.logger.setLevel(logging.ERROR)
    timestamp = str(datetime.now().strftime("%Y-%m-%d:%H:%M"))
    file_title = "logs/" + "log-" + timestamp + ".log"
    output_file_handler = logging.FileHandler(file_title)
    self.logger.addHandler(output_file_handler)

  def log(self, error_message):
    """Log an error message using the logger.

    Args:
        error_message (str): the error message to print in the log
    """
    error_message = str(datetime.now()) + "  " + error_message
    self.logger.error(error_message)
