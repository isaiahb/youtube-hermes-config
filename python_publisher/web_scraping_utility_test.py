"""Test file for web_scraping_utility.py"""
import unittest
import logs.logger
from web_scraping_utility import web_scraping_utility

logger = logs.logger.Logger()

class TestsWebUtils(unittest.TestCase):
  """Test methods from web_scraping_utility.py

  Args:
    unittest (unittest.TestCase): unittest Testcase
  """
  def test_setup_webdriver_open_window(self):
    """Test the method quit_scrape()
    Assert that the driver is set to None.
    """
    web_driver = web_scraping_utility.WebScrapingUtility(logger)
    driver = web_driver.setup_webdriver()
    assert driver is None

  def test_quit_scrape(self):
    """Test the method quit_scrape()
    Assert that the driver is set to None.
    """
    web_driver = web_scraping_utility.WebScrapingUtility(logger)
    web_driver.quit_scrape()
    driver = web_driver.driver
    assert driver is None

  def test_scrape_issues_bogus_url(self):
    """#Test quit_scrape when driver has not been set up.
    #Assert that program returns proper alert message.
    """
    web_driver = web_scraping_utility.WebScrapingUtility(logger)
    url = "1337"
    issues = web_driver.scrape_issues(url)
    web_driver.quit_scrape()
    assert len(issues) == 0

  def test_scrape_issues_not_buganizer(self):
    """#Test quit_scrape when driver has not been set up.
    #Assert that program returns proper alert message.
    """
    web_driver = web_scraping_utility.WebScrapingUtility(logger)
    url = "https://www.google.com"
    issues = web_driver.scrape_issues(url)
    web_driver.quit_scrape()
    assert len(issues) == 0

  def test_scrape_issues(self):
    """#Test quit_scrape when driver has not been set up.
    #Assert that program returns proper alert message.
    """
    web_driver = web_scraping_utility.WebScrapingUtility(logger)
    url = "https://b.corp.google.com/issues?q=componentid:920219%20status:open"
    issues = web_driver.scrape_issues(url)
    web_driver.visit_all_issues_in_list(issues)
    web_driver.quit_scrape()
    assert len(issues) != 0

if __name__ == '__main__':
  unittest.main()
