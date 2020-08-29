"""This module holds the WebScrapingUtility class which does all Buganizer html scraping
for issues under a componentid.
"""
import time
from selenium import webdriver, common
from selenium.webdriver.common.keys import Keys
import constants

class WebScrapingUtility():
  """Responsible for all Buganizer html scraping."""
  def __init__(self, logger):
    """Setup the WebScrapingUtility.

    Args:
        logger (logs.logger.Logger): the systems error logger
    """
    self.driver = self.setup_webdriver()
    self.logger = logger

  def setup_webdriver(self):
    """Completes all neccessary setup for the selenium web driver.

        Returns:
            selenium.webdriver.chrome.webdriver.WebDriver: the fully loaded Chrome
                                                          web driver with the desired profile.
    """
    try:
      options = webdriver.ChromeOptions()
      options.add_argument("user-data-dir=" + constants.PROFILE_PATH)
      driver = webdriver.Chrome(executable_path=constants.DRIVER_PATH,
                                options=options)
      return driver
    except common.exceptions.WebDriverException:
      error_message = "ERROR: Failed to load Chrome Driver. Check "\
        "path in constants.py and make sure there are no open windows with the desired profile.\n"
      self.logger.log(error_message)
      return None
    except Exception:
      return None

  def get_issue(self, issue):
    """Open the Buganizer issue with the selenium webdriver.

    Args:
        issue (str): the URL of the Buganizer issue
    """
    self.driver.implicitly_wait(3)
    self.driver.get(issue)


  def set_assignee_to_reporter(self, reporter):
    """Sets the Assignee value in the current Buganizer page to the reporter value of the issue.

    Args:
        reporter (str): the reporter of the Buganizer issue
    """
    make_assignee_editable = self.driver.find_element_by_xpath("/html/body/b-service-bootstrap/"\
      "app-root/div[7]/div/div/edit-issue-page/b-resolving-issue-references/div[2]/div[1]/div[3]"\
        "/b-resizable-sidebar/div/div[1]/div/div/div[6]/div[1]/div[1]/div[1]/div[2]")
    make_assignee_editable.click()
    assignee_input_field = self.driver.find_element_by_xpath("/html/body/b-service-bootstrap/app"\
      "-root/div[7]/div/div/edit-issue-page/b-resolving-issue-references/div[2]/div[1]/div[3]/"\
        "b-resizable-sidebar/div/div[1]/div/div/div[6]/div[1]/div[2]/div/div[1]/input")
    assignee_input_field.clear()
    assignee_input_field.send_keys(reporter)
    assignee_input_field.send_keys(Keys.ENTER)
    assignee_submit_button = self.driver.find_element_by_xpath("/html/body/b-service-bootstrap/"\
      "app-root/div[7]/div/div/edit-issue-page/b-resolving-issue-references/div[2]/div[1]/div[3]/"\
        "b-resizable-sidebar/div/div[1]/div/div/div[6]/div[1]/div[2]/div/div[2]/b-button[1]/button")
    assignee_submit_button.click()


  def print_error_to_buganizer(self, error_message):
    """Sets the Assignee value in the current Buganizer page to the specified reporter.

    Args:
        reporter (str): the reporter of the Buganizer issue
    """
    text_area = self.driver.find_element_by_xpath("/html/body/b-service-bootstrap/app-root/div[7]/"\
      "div/div/edit-issue-page/b-resolving-issue-references/div[2]/div[1]/div[3]/div/div/div[2]/"\
        "div[3]/div/bv2-comment-box/div/div[1]/b-comment-draft/b-comment-draft-textarea/textarea")
    text_area.send_keys(error_message)
    comment_button = self.driver.find_element_by_xpath("/html/body/b-service-bootstrap/app-root/"\
      "div[7]/div/div/edit-issue-page/b-resolving-issue-references/div[2]/div[1]/div[3]/div/div/"\
        "div[2]/div[3]/div/bv2-comment-box/div/div[3]/div[1]/div/b-button/button")

    while not comment_button.is_enabled():
      time.sleep(.1)
    comment_button.click()

  def post_image(self, image_path):
    """Upload an image to the current Buganizer issue.

    Args:
        image_path (str): the file path of the image
    """
    upload_button = self.driver.find_element_by_xpath("/html/body/b-"\
      "service-bootstrap/app-root/div[7]/div/div/edit-issue-page/b-resolving-"\
        "issue-references/div[2]/div[1]/div[3]/div/div/div[2]/div[3]/div/bv2-"\
          "comment-box/div/div[3]/div[2]/div/div[2]/input")

    upload_button.send_keys(image_path)

    comment_button = self.driver.find_element_by_xpath("/html/body/b-service-bootstrap"\
      "/app-root/div[7]/div/div/edit-issue-page/b-resolving-issue-references/div[2]/"\
        "div[1]/div[3]/div/div/div[2]/div[3]/div/bv2-comment-box/div/div[3]/div[1]"\
          "/div/b-button/button")

    while not comment_button.is_enabled():
      time.sleep(.1)
    comment_button.click()


  def mark_as_fixed(self):
    """Mark the Status of the current Buganizer to Fixed.
    """
    status_button = self.driver.find_element_by_xpath("/html/body/b-service-bootstrap/"\
      "app-root/div[7]/div/div/edit-issue-page/b-resolving-issue-references/div[2]/div[1]"\
        "/div[3]/b-resizable-sidebar/div/div[1]/div/div/span/span/div/div[1]/div[1]/div[2]/"\
          "div[1]/truncated-span")
    status_button.click()
    fixed_button = self.driver.find_element_by_xpath("/html/body/b-service-bootstrap/app-root/"\
      "div[7]/div/div/edit-issue-page/b-resolving-issue-references/div[2]/div[1]/div[3]/"\
        "b-resizable-sidebar/div/div[1]/div/div/span/span/div/div[2]/div/div[1]/div/div[4]")
    fixed_button.click()

  def get_issue_status(self):
    """Retrieve the Status value from the current Buganizer issue.

    Returns:
        str: the Status value from the current Buganizer issue
    """
    status_button = self.driver.find_element_by_xpath("/html/body/b-service-bootstrap/app-root/"\
      "div[7]/div/div/edit-issue-page/b-resolving-issue-references/div[2]/div[1]/div[3]/"\
        "b-resizable-sidebar/div/div[1]/div/div/span/span/div/div[1]/div[1]/div[2]/div[1]/"\
          "truncated-span/span")
    return status_button.text

  def get_issue_assignee(self):
    """Retrieve the Assignee value from the current Buganizer issue.

    Returns:
       str : the Status value from the current Buganzier issue
    """
    assignee_tag = self.driver.find_element_by_xpath("/html/body/b-service-bootstrap/app-root/"\
      "div[7]/div/div/edit-issue-page/b-resolving-issue-references/div[2]/div[1]/div[3]/"\
        "b-resizable-sidebar/div/div[1]/div/div/div[6]/div[1]/div[1]/div[1]/div[2]/div[1]/"\
          "div/b-user-membership-chip/span/span/span")
    return assignee_tag.text
