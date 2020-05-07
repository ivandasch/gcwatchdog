[![Build Status](https://travis-ci.com/ivandasch/gcwatchdog.svg?branch=master)](https://travis-ci.com/ivandasch/gcwatchdog)
# Simple JVMTI agent to monitor long GC pauses.
## Usage
Add these to JVM options.
 ```
-agentpath:<path_to_agentlib>=threshold=<milliseconds>[default: 100],heapDumpOnExit=<true|false>[default: false],heapDumpPath=<path_to_dir>[default: '.'],logFilePath=<path_to_file>
```

Example:
```
-agentpath:<path_to_agentlib>=threshold=500,heapDumpOnExit=true,heapDumpPath=/tmp,logFilePath=/var/log/agent.log
```
## Build
`Cmake >= 3.14`, `gcc >= 4.8.1` required. `JDK` also must presents.

Run following commands:
```
mkdir build
cd build
cmake ..
make
make test
cpack
```