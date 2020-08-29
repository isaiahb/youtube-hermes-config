# YouTube Hermes Config Automation

## **Project Goal:**
The objective of this system is to replace preexisting YouTube Hermes configuration tools with a user friendly UI for Eng /Ops members and vendor management to make configuration change requests. The UI must automatically send all new configuration change requests to the backend.  This UI must have the ability to provide the user with responses received from the backend, such as images and text files. <br/><br/>

## Overview:<br/>
With the new system, Eng/Ops members and vendor management personnel are able to create and edit Pacing system configuration change requests from the familiar Buganizer UI, instead of using the old QueueConfiguration UI.
Users will start from either the EnqueueRules UI, the RoutingTargetsUI or the QueueInfo UI. When they choose a configuration that they would like to change, a new Buganizer issue draft is opened with the pre-existing configuration data already filled out. They then just have to change the fields that they would like to change and then create the bug.<br/><br/>
Once the bug is created, the request is automatically sent to CAT where it is processed and compared to historical traffic data, and an impact analysis response is generated. If the request was invalid, Buganizer receives and prints a message detailing why the change request was invalid.  If the request was valid, then an impact analysis graph is created and included along with a detailed response of the impact that these changes will have. If applicable, anyone listed under the CC field will be notified of the change.
<br/> <br/>


**Setup Instructions:**
-------------------------------------------------------------------------------

**Authentication:**

This program requires authentication to be setup. Refer to the
`Authentication Getting Started Guide` for instructions on how to set up
credentials for applications.

*Authentication Getting Started Guide:*
    https://cloud.google.com/docs/authentication/getting-started
    
<br/>




## Installing & Logging into gcloud from terminal

1. *Installing gcloud:* https://cloud.google.com/sdk/install
2. *Using gcloud to set the project and login to authorize:* https://cloud.google.com/sdk/gcloud/reference/auth/login
 
<br>

## Install gcloud on Linux
	$ curl https://sdk.cloud.google.com
	
	$ bash install.sh
 
 <br>

## Installing on Mac os
Download and follow instructions here. https://cloud.google.com/sdk/docs/quickstart-macos
 
## gcloud Auth
	$ gcloud auth login
	
	$ gcloud config set project google.com:youtube-admin-pacing-server

<br><br>

**Install Dependencies**

1. Clone the project repository in whatever directory you want to use.

        $ git clone https://github.com/viktries-google/youtube-hermes-config
        
2. Install the latest version of Python, as well as pip. Refer to `Python Development Environment Setup Guide` for Google Cloud Platform for further instructions.

   *Python Development Environment Setup Guide:* https://cloud.google.com/python/setup

        $ sudo apt update
        $ sudo apt install python3 python3-dev python3-venv
        $ wget https://bootstrap.pypa.io/get-pip.py
        $ sudo python3 get-pip.py
        $ pip --version

3. Create a virtualenv. For more info: https://virtualenv.pypa.io/

        $ cd youtube-hermes-config
        $ python3 -m venv venv
        $ source venv/bin/activate

4. Install the dependencies needed to run the program. For more info: https://pip.pypa.io/

        $ pip install bs4
        $ pip install --upgrade google-cloud-pubsub
        $ pip install --upgrade google-cloud-spanner
        $ pip install selenium
        $ pip install matplotlib
        $ pip install pandas
    
**Set up Protocol Buffer**

 First view instructions and download the protocol buffer compiler at : https://developers.google.com/protocol-buffers
 

        $ PROTOC_ZIP=protoc-3.7.1-linux-x86_64.zip curl -OL https://github.com/protocolbuffers/protobuf/releases/download/v3.7.1/$PROTOC_ZIP 
        $ sudo unzip -o $PROTOC_ZIP -d /usr/local bin/protoc
        $ sudo unzip -o $PROTOC_ZIP -d /usr/local 'include/*'
        $ rm -f $PROTOC_ZIP
        $ protoc -I=. --python_out=python_publisher/config_change_request/ ./config_change.proto
        $ protoc -I=. --python_out=python_subscriber/ ./config_change.proto
        $ protoc -I=. --python_out=python_subscriber/ ./impact_analysis_response.proto


 




**Download Chrome Driver:**<br/>
Download the Chromedriver here https://chromedriver.chromium.org/downloads to whatever directory you would like.


<br><br>
**C++ Subscriber Setup Instructions:**
-------------------------------------------------------------------------------
<br>

## Installing bazel on Ubuntu
	$ sudo apt update && sudo apt install bazel
 
## Installing bazel on Mac os with Homebrew
	$ brew install bazel
 
<br><br>

## Installing & Logging into gcloud from terminal

1. *Installing gcloud:* https://cloud.google.com/sdk/install
2. *Using gcloud to set the project and login to authorize:* https://cloud.google.com/sdk/gcloud/reference/auth/login
 
<br>

## Install gcloud on Linux
	$ curl https://sdk.cloud.google.com
	
	$ bash install.sh
 
 <br>

## Installing on Mac os
Download and follow instructions here. https://cloud.google.com/sdk/docs/quickstart-macos
 
## gcloud Auth
	$ gcloud auth login
	
	$ gcloud config set project google.com:youtube-admin-pacing-server
 


<br><br>
**Run**
-------------------------------------------------------------------------------

 
**C++ Subscriber:**
 
Run with bazel.
 
	$ bazel run //cpp_subscriber:main
 
<br><br>
 

** Python Run Instructions:**<br/><br/>
Before running **make sure to edit *python_publisher/constants.py* and *python_subscriber/constants.py*** as needed. **You will need to create two different Chrome profiles** and enter their file paths into the respective constants.py file. To find the file path visit *chrome://version* and look under **Profile Path**.
<br/><br/>Once you are ready to run, open three terminal windows. Make sure there are **no open Chrome windows** with the profiles that you are using.<br/><br/>
Python Scrapper:

        $ cd python_subscriber
        $ python3 main.py   
<br/>

Python Subscriber:

        $ cd python_publisher
        $ python3 main.py


**Note:** The first time you run the project each 24hr period you will be brought to MOMA Single Sign on. Select the 'Use Security Code' option and **generate a security code at go/sc to log in**. Once you are logged in you will be brought to Buganizer.