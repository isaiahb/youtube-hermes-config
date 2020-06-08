# Template for Hermes configuration change request

## Three different configurations to change
* Enqueue Rule
* Routing Rule
* Queue Info

---
## Enqueue Rules Template 

### Example
```
Configuration: EnqueueRule
Features: f1, f2, f3, f4, f5, f6
Queue: Q3
```

---
## Routing Rules Template

### Example
```
Configuration: RoutingRule
Queue: Q2
Possible-Routes: Q3, Q4, Q5, Q6
```

---
## Queue Info Template

### Example
```
Configuration: QueueInfo
Queue: Q2
Owners: isaiah@google.com, saulo@google.com
```
