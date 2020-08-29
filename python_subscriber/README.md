# Buganizer Impact Response Subscriber

## **Overview:**
The Buganizer subscriber subsystem constantly pulls messages from a given Pub/Sub topic. These messages contain serialized Impact Analysis Response protobuf objects. The subscriber will process the messages one at a time, so that if there are multiple configuration requests from the same issue, they will never be processed concurrently, and the issue will be closed or invalidated accordingly. Processing each message means that the subscriber will deserialize the protobuf object and check if there are affected queues and/or an error message. If there are affected queues and there is no error message (EnqueueRules or RoutingTargets success), then an impact analysis graph will be generated and posted to Buganizer. If there are no affected queues and no error message (QueueInfo success), then a change log will be uploaded to Buganizer. And finally if there are no affected queues and there exists an error message (unsuccessful), then a detailed message explaining why the request is invalid will be uploaded to Buganizer. <br/><br/>


**Configuration**
-------------------------------------------------------------------------------

**Edit constants.py:**<br/>
SUBSCRIPTION_ID: *The subscription id of the Pub/Sub topic.*<br/>
PROJECT_ID: *The Project ID from GCP*<br/>
TOPIC_NAME: *The name of the Pub/Sub Topic* <br/>
URL: *The URL containing one or more Buganizer issues under a componentid.*<br/>
PROJECT_PATH: *Path to the root directory of the project.* <br/>
DRIVER_PATH: *Path to the Chromedriver.*<br/>
PROFILE_PATH: *Path to the Chrome profile you would like to use. Visit chrome://version if you are unsure.*<br/>
AUTOMATION_USER: *The special automation user that controls automation.*<br/>


**Logs**
-------------------------------------------------------------------------------


**Error Logging** Each time main.py is run, a new timestamped log file is created in the logs/ directory. Any system error messages will be printed in this log file.