"""This module holds the WebScrapingUtility class which does all Buganizer html scraping
  for issues under a componentid.
"""
import time
from bs4 import BeautifulSoup
from selenium import webdriver, common
from message_parsing_utility import message_parsing_utility
import constants

class WebScrapingUtility():
  """Responsible for all Buganizer html scraping."""
  def __init__(self, logger):
    """Setup the WebScrapingUtility

    Args:
        logger (logs.logger.Logger): the systems error logger
    """
    self.driver = self.setup_webdriver()
    self._message_parsing_util = message_parsing_utility.MessageParsingUtility(logger)
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

  def quit_scrape(self):
    """Terminate the webdriver.
    """
    if self.driver:
      self.driver.quit()
      self.driver = None

  def scrape_issues(self, url):
    """Opens the Buganizer url in the Chrome Browser and scrapes the webpage's source html
        to get the links for all the issues under the componentid.

        Args:
            url (str): the Buganizer url to scrape

        Return:
            list : List with all the buganizer issues found under the componentid.
    """
    try:
      self.driver.get(url)
    except common.exceptions.InvalidSessionIdException:
      self.driver.close()
      error_message = "ERROR: Failed to reach URL, check "\
      "specified URL in constants.py\n"
      self.logger.log(error_message)
      return []
    except Exception:
      self.driver.close()
      error_message = "ERROR: Failed to reach URL, check "\
      "specified URL in constants.py\n"
      self.logger.log(error_message)
      return []

    source_html = self.driver.page_source
    soup = BeautifulSoup(source_html, "html.parser")
    page_title = soup.title.string
    buganizer_issues = []

    if "Buganizer" not in page_title or "componentid" not in page_title:
      if "MOMA Single Sign On" in page_title:
        error_message = "ERROR: You must log into your MOMA account "\
        "first. Select the 'Use Security Code' option and generate a security code at go/sc.\n"
        self.logger.log(error_message)

        while "Buganizer" not in page_title:
          source_html = self.driver.page_source
          soup = BeautifulSoup(source_html, "html.parser")
          page_title = soup.title.string
          time.sleep(1)

        return buganizer_issues
      error_message = "ERROR: URL does not link to a Buganizer "\
        "componentid, check specified URL "\
        "in constants.py\n"
      self.logger.log(error_message)
      return buganizer_issues

    for tbody in soup.find_all('tbody'):
      for _tr in tbody.find_all('tr'):
        issue_link = "https://b.corp.google.com/issues/" + _tr.get(
            'data-row-id')
        buganizer_issues.append(issue_link)
    return buganizer_issues

  def visit_all_issues_in_list(self, issues):
    """
    From the list of buganizer issues, visit each issue with the webdriver,
    find the reporter from an html tag send issue html to message_parsing_utility to be parsed.

    Args:
            issues (list): the Buganizer urls to scrape
    """
    for issue in issues:
      self.driver.implicitly_wait(3)
      self.driver.get(issue)
      config_type_text = self.driver.find_element_by_xpath("/html/body/b-service-bootstrap/"\
        "app-root/div[7]/div/div/edit-issue-page/b-resolving-issue-references/div[2]/div[1]/"\
          "div[3]/div/div/div[2]/div[2]/div[3]/div/div[1]/div/span/span[6]/span/span/a").text

      source_html = self.driver.page_source
      soup = BeautifulSoup(source_html, "html.parser")

      advanced_fields = {}
      advanced_fields["Issue Id"] = issue.replace("https://b.corp.google.com/issues/", "")
      reporter_tag = soup.find("div", "bv2-issue-metadata-field-inner "\
        "bv2-issue-metadata-field-reporter")
      reporter = reporter_tag["aria-label"].replace(
          " value is ", "\n").split("\n")
      advanced_fields[reporter[0]] = reporter[1]
      assignee_tag = soup.find("div", "bv2-issue-metadata-field-inner bv2-issue-metadata-"\
        "field-assignee")
      assignee = assignee_tag["aria-label"].replace(
          " value is ", "\n").split("\n")
      if assignee[1] != "empty":
        advanced_fields[assignee[0]] = assignee[1]

      if "EnqueueRule" in config_type_text:
        config_type = "EnqueueRules"
      elif "RoutingTargets" in config_type_text:
        config_type = "RoutingTargets"
      elif "QueueInfo" in config_type_text:
        config_type = "QueueInfo"

      advanced_fields["Config Type"] = config_type

      if config_type == "QueueInfo":
        if assignee[1] != constants.AUTOMATION_USER:
          continue

        self.scrape_queue_info(advanced_fields)
      elif config_type == "RoutingTargets":
        if assignee[1] != constants.AUTOMATION_USER:
          continue
        self.scrape_routing_targets(advanced_fields)
      elif config_type == "EnqueueRules":
        self._message_parsing_util.parse_page(soup, reporter[1], issue)

  def scrape_queue_info(self, advanced_fields):
    """Scrape all advanced fields from a RoutingTarget Buganizer Issue

    Args:
        advanced_fields (dict): dictionary that holds all advanced fields values
    """
    source_html = self.driver.page_source
    soup = BeautifulSoup(source_html, "html.parser")
    severity_tag = soup.find("div", "bv2-issue-metadata-field-inner "\
      "bv2-issue-metadata-field-severity")
    severity = severity_tag["aria-label"].replace(
        " value is ", "\n").split("\n")
    found_in_tag = soup.find("div", "bv2-issue-metadata-field-inner "\
      "bv2-issue-metadata-field-foundInVersion")
    found_in = found_in_tag["aria-label"].replace(
        " value is ", "\n").split("\n")
    in_prod_tag = soup.find("div", "bv2-issue-metadata-field-inner "\
      "bv2-issue-metadata-field-inProd")
    in_prod = in_prod_tag["aria-label"].replace(
        " value is ", "\n").split("\n")
    verifier_tag = soup.find("div", "bv2-issue-metadata-field-inner "\
      "bv2-issue-metadata-field-verifier")
    verifier = verifier_tag["aria-label"].replace(
        " value is ", "\n").split("\n")
    targeted_to_tag = soup.find("div", "bv2-issue-metadata-field-inner "\
      "bv2-issue-metadata-field-targetedToVersion")
    targeted_to = targeted_to_tag["aria-label"].replace(
        " value is ", "\n").split("\n")
    queue_id_tag = soup.find("div", "bv2-issue-metadata-field-inner "\
      "bv2-issue-metadata-field-customField688197")
    queue_id = queue_id_tag["aria-label"].replace(
        " value is ", "\n").split("\n")
    mdb_group_name_tag = soup.find("div", "bv2-issue-metadata-field-inner "\
      "bv2-issue-metadata-field-customField686879")
    mdb_group_name = mdb_group_name_tag["aria-label"].replace(
        " value is ", "\n").split("\n")
    ops_owner_tag = soup.find("div", "bv2-issue-metadata-field-inner "\
      "bv2-issue-metadata-field-customField686850")
    ops_owner = ops_owner_tag["aria-label"].replace(
        " value is ", "\n").split("\n")
    gvo_owner_tag = soup.find("div", "bv2-issue-metadata-field-inner "\
      "bv2-issue-metadata-field-customField686358")
    gvo_owner = gvo_owner_tag["aria-label"].replace(
        " value is ", "\n").split("\n")
    tech_owner_tag = soup.find("div", "bv2-issue-metadata-field-inner "\
      "bv2-issue-metadata-field-customField686980")
    tech_owner = tech_owner_tag["aria-label"].replace(
        " value is ", "\n").split("\n")
    is_dashboard_queue_tag = soup.find("div", "bv2-issue-metadata-field-inner "\
      "bv2-issue-metadata-field-customField686718")
    is_dashboard_queue = is_dashboard_queue_tag["aria-label"].replace(
        " value is ", "\n").split("\n")
    reviews_per_item_tag = soup.find("div", "bv2-issue-metadata-field-inner "\
      "bv2-issue-metadata-field-customField687560")
    reviews_per_item = reviews_per_item_tag["aria-label"].replace(
        " value is ", "\n").split("\n")
    fragment_name_tag = soup.find("div", "bv2-issue-metadata-field-inner "\
      "bv2-issue-metadata-field-customField686833")
    fragment_name = fragment_name_tag["aria-label"].replace(
        " value is ", "\n").split("\n")
    item_expiry_sec_tag = soup.find("div", "bv2-issue-metadata-field-inner "\
      "bv2-issue-metadata-field-customField686748")
    item_expiry_sec = item_expiry_sec_tag["aria-label"].replace(
        " value is ", "\n").split("\n")
    is_experimental_review_enabled_tag = soup.find("div", "bv2-issue-metadata-field-inner "\
      "bv2-issue-metadata-field-customField688166")
    is_experimental_review_enabled = is_experimental_review_enabled_tag["aria-label"].replace(
        " value is ", "\n").split("\n")
    experimental_probability_tag = soup.find("div", "bv2-issue-metadata-field-inner "\
      "bv2-issue-metadata-field-customField686699")
    experimental_probability = experimental_probability_tag["aria-label"].replace(
        " value is ", "\n").split("\n")

    advanced_fields[severity[0]] = severity[1]
    if verifier[1] != "empty":
      advanced_fields[verifier[0]] = verifier[1]
    if found_in[1] != "empty":
      advanced_fields[found_in[0]] = found_in[1]
    if in_prod[1] != "empty":
      if in_prod[1] == "Yes":
        advanced_fields[in_prod[0]] = True
      else:
        advanced_fields[in_prod[0]] = False
    if targeted_to[1] != "empty":
      advanced_fields[targeted_to[0]] = targeted_to[1]
    if queue_id[1] != "empty":
      advanced_fields[queue_id[0]] = int(queue_id[1])
    if mdb_group_name[1] != "empty":
      advanced_fields[mdb_group_name[0]] = mdb_group_name[1]
    if ops_owner[1] != "empty":
      advanced_fields[ops_owner[0]] = ops_owner[1]
    if gvo_owner[1] != "empty":
      advanced_fields[gvo_owner[0]] = gvo_owner[1]
    if tech_owner[1] != "empty":
      advanced_fields[tech_owner[0]] = tech_owner[1]
    if is_dashboard_queue[1] != "empty":
      if is_dashboard_queue[1] == "true":
        advanced_fields[is_dashboard_queue[0]] = True
      else:
        advanced_fields[is_dashboard_queue[0]] = False
    if reviews_per_item[1] != "empty":
      advanced_fields[reviews_per_item[0]] = int(reviews_per_item[1])
    if fragment_name[1] != "empty":
      advanced_fields[fragment_name[0]] = fragment_name[1]
    if item_expiry_sec[1] != "empty":
      advanced_fields[item_expiry_sec[0]] = int(item_expiry_sec[1])
    if is_experimental_review_enabled[1] != "empty":
      if is_dashboard_queue[1] == "true":
        advanced_fields[is_experimental_review_enabled[0]] = True
      else:
        advanced_fields[is_experimental_review_enabled[0]] = False
    if experimental_probability[1] != "empty":
      advanced_fields[experimental_probability[0]] = int(experimental_probability[1])

    self._message_parsing_util.publish_buganizer_fields(advanced_fields)

  def scrape_routing_targets(self, advanced_fields):
    """Scrape all advanced fields from a RoutingTarget Buganizer Issue

    Args:
        advanced_fields (dict): dictionary that holds all advanced fields values
    """
    source_html = self.driver.page_source
    soup = BeautifulSoup(source_html, "html.parser")
    try:
      show_all = self.driver.find_element_by_id("bv2-issue-metadata-list-4-more")
      show_all.click()
      show_all = self.driver.find_element_by_id("bv2-issue-metadata-list-5-more")
      show_all.click()
    except common.exceptions.NoSuchElementException:
      pass

    severity_tag = soup.find("div", "bv2-issue-metadata-field-inner "\
      "bv2-issue-metadata-field-severity")
    severity = severity_tag["aria-label"].replace(
        " value is ", "\n").split("\n")
    found_in_tag = soup.find("div", "bv2-issue-metadata-field-inner "\
      "bv2-issue-metadata-field-foundInVersion")
    found_in = found_in_tag["aria-label"].replace(
        " value is ", "\n").split("\n")
    in_prod_tag = soup.find("div", "bv2-issue-metadata-field-inner "\
      "bv2-issue-metadata-field-inProd")
    in_prod = in_prod_tag["aria-label"].replace(
        " value is ", "\n").split("\n")
    verifier_tag = soup.find("div", "bv2-issue-metadata-field-inner "\
      "bv2-issue-metadata-field-verifier")
    verifier = verifier_tag["aria-label"].replace(
        " value is ", "\n").split("\n")
    targeted_to_tag = soup.find("div", "bv2-issue-metadata-field-inner "\
      "bv2-issue-metadata-field-targetedToVersion")
    targeted_to = targeted_to_tag["aria-label"].replace(
        " value is ", "\n").split("\n")
    queue_id_tag = soup.find("div", "bv2-issue-metadata-field-inner "\
      "bv2-issue-metadata-field-customField688193")
    queue_id = queue_id_tag["aria-label"].replace(
        " value is ", "\n").split("\n")

    queues_to_add = []
    queues_to_remove = []

    for tag in soup.find_all("button", id=lambda value: value and value.startswith(
            "bv2-issue-metadata-list")):
      queue_method_string = tag["aria-label"]
      if "Add Queues to Route To" in queue_method_string:
        queue_method_string = queue_method_string.replace("Remove ", "")
        queue_method_string = queue_method_string.replace(" from Add Queues to Route To", "")
        queues_to_add.append(int(queue_method_string))
      elif "Remove Queues to Route To" in queue_method_string:
        queue_method_string = queue_method_string.replace("Remove ", "")
        queue_method_string = queue_method_string.replace(" from Queues to Route To", "")
        queues_to_remove.append(int(queue_method_string))

    advanced_fields["Add Queues to Route To"] = queues_to_add
    advanced_fields["Remove Queues to Route To"] = queues_to_remove

    advanced_fields[severity[0]] = severity[1]
    if verifier[1] != "empty":
      advanced_fields[verifier[0]] = verifier[1]
    if found_in[1] != "empty":
      advanced_fields[found_in[0]] = found_in[1]
    if in_prod[1] != "empty":
      if in_prod[1] == "Yes":
        advanced_fields[in_prod[0]] = True
      else:
        advanced_fields[in_prod[0]] = False
    if targeted_to[1] != "empty":
      advanced_fields[targeted_to[0]] = targeted_to[1]
    if queue_id[1] != "empty":
      advanced_fields[queue_id[0]] = int(queue_id[1])

    self._message_parsing_util.publish_buganizer_fields(advanced_fields)
