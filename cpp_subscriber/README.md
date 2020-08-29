# YouTube Hermes Config Automation
## C++ Pub/Sub Subscriber
 
## Functionality:<br/>
This program creates and runs a Pub/Sub client object which subscribes to a specified Pub/Sub topic containing a configuration change request. Once it recives a message it gets processed in a callback function to evaluate the change request.
 
<br/><br/>
 
**Setup Instructions:**
-------------------------------------------------------------------------------
 
**Authentication:**
 
This program requires authentication to be set up. Refer to the
`Authentication Getting Started Guide` for instructions on how to set up
credentials for applications.
 
*Authentication Getting Started Guide:*
   https://cloud.google.com/docs/authentication/getting-started
  
<br/>
 
**Install Dependencies**
 
Clone the project repository
 
	$ git clone https://github.com/viktries-google/youtube-hermes-config

<br>

Install `bazel` a build tool used to build and run this project and its dependencies
  *Installing Bazel:* https://docs.bazel.build/versions/master/install.html
 
## Installing bazel on Ubuntu
	$ sudo apt update && sudo apt install bazel
 
## Installing bazel on Mac os with Homebrew
	$ brew install bazel
 
 
<br><br>
      
## Set Environment Variables, or use gcloud from terminal to login
### Setting environment variables and google service account json key.
 
	$ export PROJECT='[PROJECT_ID]'
	
	$ export GOOGLE_APPLICATION_CREDENTIALS=~=/Downloads/[SERVICE_ACCOUNT_JSON_KEY]
 
<br>
 
#### Installing & Logging into gcloud from terminal
1. *Installing gcloud:* https://cloud.google.com/sdk/install
2. *Using gcloud to set thse project and login to authorize:* https://cloud.google.com/sdk/gcloud/reference/auth/login
 
## Install gcloud on Linux
	$ curl https://sdk.cloud.google.com
	
	$ bash install.sh
 
## Installing on Mac os
Download and follow instructions here. https://cloud.google.com/sdk/docs/quickstart-macos
 
## Login & set project
	$ gcloud auth login
	
	$ gcloud config set project google.com:youtube-admin-pacing-server
 
<br/>
<br/>
 
**Run**
-------------------------------------------------------------------------------
 
**Run Instructions:**
 
From terminal make sure you are in the `/cpp_subscriber` folder
 
	$ cd cpp_subscriber
Run with bazel.
 
	$ bazel run :main
 
**Run Unit Test:**
	$ bazel test :unit_test
<br>
   ***note:***
 
On Mac os to run unit tests, use command.
 
	$ bazel test --copt -DGRPC_BAZEL_BUILD :unit_test
      
<br><br>
 
**Program Architecture**
-------------------------------------------------------------------------------
 
The program is composed of the following components.
 
- Pub/Sub Subscriber
- Message Processor
- Impact Analysis
- Pub/Sub Publisher (Impact Analysis Response)
 
```
[Pub/Sub Subscriber] -> [Message Processor] -> [Impact Analysis] -> [Pub/Sub Publisher]
 
[Historical Spanner DB] -> [Impact Analysis]
```
<br>

# Component Roles
## Pub/Sub Subscriber (Client)
The clients job is to subscribe to a specified Pub/Sub topic in order to receive configuration change requests. The cloud Pub/Sub topic will receive messages from the Pub/Sub publisher containing a serialized ConfigChangeRequest protobuf object and pass it to the Message Processor.
 
 
## Message Processor
The Message Processor component is in charge of processing the Pub/Sub Message. This involves first extracting the serialized ConfigChangeRequest object from the message data field, deserializing the object, and passing it to the Impact Analysis component.
 
## Impact Analysis
The Impact analysis component is responsible for receiving a ConfigChangeRequest object, connecting it to the Historical Spanner Database in order to get historical video traffic, and calculating the impact of the change on the system. The Impact Analysis will then create a ImpactAnalysisResult protobuf object, and pass it to the Pub/Sub Publisher component.
 
## Pub/Sub Publisher
The Pub/Sub Publisher component is responsible for receiving an ImpactAnalysisResult object, serializing it, and publishing it as a message to the specified Pub/Sub topic as a response to the original configuration change request.

