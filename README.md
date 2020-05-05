# Simple JVMTI agent to monitor long GC pauses.
## Usage
Add these to JVM options.
 ```
-agentlib:<path_to_agentlib>=threshold=<milliseconds>[default: 100],heapDumpOnExit=<true|false>[default: false],heapDumpPath=<path>[default: '.'],logFilePath=<path_to_file>"
```

Example:
```
-agentlib:<path_to_agentlib>=threshold=500,heapDumpOnExit=true,heapDumpPath=/tmp,logFilePath=/var/log/agent.log"
```