# Buganizer Scraper

## **Overview:**
Once running, this subsystem constantly iterates through various issues under a configuration type, and attempts to scrape data from any issue possible. The way that this automation will be controlled is through a special automation user. Upon creation of the Buganizer issue, it will be assigned to this automation user eg. 'hermesautomationuser@google.com'. As the system iterates through issues and comes across an issue that is open and assigned to the automation user, it will scrape all of its change request data. It will then put the change request data inside of a protobuf object (config_change.proto) that will hold all necessary fields depending on the configuration change type. Once this proto is created, it will be deserialized and published in a Pub/Sub message. The ConfigurationAutomationTool(CAT) will pull these messages where they will be processed.
 <br/><br/>


**Configuration**
-------------------------------------------------------------------------------

**Edit constants.py:**<br/>
PROJECT_ID: *The Project ID from GCP*<br/>
TOPIC_NAME: *The name of the Pub/Sub Topic* <br/>
URL: *The URL containing one or more Buganizer issues under a componentid.*<br/>
PROJECT_PATH: *Path to the root directory of the project.* <br/>
DRIVER_PATH: *Path to the Chromedriver.*<br/>
PROFILE_PATH: *Path to the Chrome profile you would like to use. Visit chrome://version if you are unsure.*<br/>
CONFIGURATION_SPECIFIER: *The specifier for all templates for the configuration type. Default = 'Configuration: '*<br/>
METHOD_SPECIFIER: *The EnqueueRules template's specifier for method. Default = 'Method'*<br/>
POSSIBLE_METHODS: *The EnqueueRules template's possible methods. Default = 'Add', 'Remove', or 'Set'*<br/>
QUEUE_SPECIFIER: *The EnqueueRules template's specifier for queue. Default = 'Queue: '*<br/>
ENQUEUE_RULE_COMMAND_LINES_COUNT: *The EnqueueRules template's number of command specifiers. Every line except Configuration. Default = 4*<br/>
ENQUEUE_RULE_FEATURES_SPECIFIER: *The EnqueueRules template's specifier for features. Default = 'Features: '*<br/>
ENQUEUE_RULE_PRIORITY_SPECIFIER: *The EnqueueRules template's specifier for priority. Default = 'Priority: '*<br/>
AUTOMATION_USER: *The special automation user that controls automation.*<br/>


**Logs**
-------------------------------------------------------------------------------


**Error Logging** Each time main.py is run, a new timestamped log file is created in the logs/ directory. Any system error messages will be printed in this log file.
