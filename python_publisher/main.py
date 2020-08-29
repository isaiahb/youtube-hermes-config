"""This module holds the System class which completes any necessary setup for scraping.
"""
import constants
import logs.logger
from web_scraping_utility import web_scraping_utility

class System():
  """Completes any necessary setup for scraping."""

  def __init__(self):
    logger = logs.logger.Logger()
    self.web_scraping_util = web_scraping_utility.WebScrapingUtility(logger)

  def begin_scrape(self, url):
    """Begins the proccess of scraping Buganizer.

    Args:
      url (str): the Buganizer url to scrape
    """
    while True:
      issues = self.web_scraping_util.scrape_issues(url)

      if len(issues) > 0:
        self.web_scraping_util.visit_all_issues_in_list(issues)

if __name__ == "__main__":
  system = System()
  if system.web_scraping_util.driver:
    system.begin_scrape(constants.URL)
