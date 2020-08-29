

# Spanner Database

## **Overview:**
The spanner population subsystem fills the database with random sample data that simulates various components in the real system.

 In order to simulate entities that are enqueued in the system, there exists tables for:<br/>
 * Videos<br/>
 * Queues <br/>
* Enqueue Rule<br/>

This data is populated in a way that ensures maximum randomness within constraints. Each video entry has a random list of features, and each enqueue rule entry has a random list of features(rules) for a queue id, that will simulate determining what queue videos with those features will be enqueued to. 

 In addition, to simulate the review queue state (entity getting enqueued, routed or verdict is made), there exists tables for:<br/>
 * Enqueue Signal<br/>
 * Routed Signal<br/>
 * Verdict Signal <br/>

This data is populated in a way that ensures maximum randomness, while simulating realistic life cycles of entities. In order for life cycles to be realistic, EnqueueSignal timestamps must be earlier than corresponding RoutedSignal entries if they were routed, and VerdictSignal entries if they were not routed. If it was routed and a verdict was reached, the timestamp for the RoutedSignal must be earlier than the VerdictSignal. Queue Id fields for these cases must also reflect correct movement from queue to queue. If an entity is routed, the from-queue must match the queue id for the enqueue signal of that entity. If a verdict signal is made the queue id must either match that of the respective enqueue signal if it was not routed, or the to-queue of the routed signal if it was routed.  In the real system, an entity could be routed multiple times, however, for simplicity, we will be assuming entities can only be routed once at most.
 <br/><br/>

## Functionality:<br/>
Running this program will clear all entries previously existing in all tables, and then fill all tables with the sample data described above.<br/><br/>



**Configuration**
-------------------------------------------------------------------------------

**Edit constants.py:**<br/>
INSTANCE_ID: *The instance id of the spanner database.*<br/>
DATABASE_ID: *The database id of the spanner database.* <br/>
ROW_COUNT: *The number of rows to create in each table.*<br/>
MAX_FEATURES_PER_VIDEO: *The maximum amount of features allowed per video.* <br/>
FEATURES_COUNT: *The total number of different video features.*<br/>
DESIRED_SLA_COUNT: *The range for desired Service Level Agreement(SLA) in minutes.*<br/>
OWNERS_COUNT: *The total number of possible queue owners.*<br/>
MAX_POSSIBLE_ROUTES: *The maximum amount of possible routes allowed per queue.*<br/>
MAX_RULES_PER_ENQUEUE_RULES: *The maximum amount of rules allowed per enqueue rule.*<br/>
RULE_COUNT: *The total number of different possible rules.*<br/>


**Run**
-------------------------------------------------------------------------------

**Run Instructions:**

Before running make sure to adjust python_publisher/constants.py and python_subscriber/constants.py as needed. In a terminal window, run  the following: <br/><br/>

        $ python3 historical_traffic.py