/*
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <jvmti.h>
#include <jni.h>
#include <thread>
#include <iostream>
#include "vm.h"
#include "gc.h"
#include "param.h"

void printHelp() {
    std::cout << "usage: -agentpath:<path_to_agentlib>=threshold=<milliseconds>[default: 100]"
              << ",heapDumpOnExit=<true|false>[default: false]"
              << ",heapDumpPath=<path_to_dir>[default: '.'],logFilePath=<path_to_file>"
              << std::endl;
}

JNIEXPORT jint JNICALL Agent_OnLoad(JavaVM *vm, char *options, void *reserved) {
    GcWatchdogParam param;
    try {
        param = parse(options);
    } catch (HelpException &e) {
        printHelp();

        return 0;
    } catch (std::runtime_error &e) {
        std::cout << e.what() << std::endl;

        printHelp();

        return 0;
    }

    if (!param.LogFilePath.empty())
        Log::Init(param.LogFilePath);

    if (param.IsHeapDumpEnabled) {
        Log::Trace("Starting gc watchdog with params treshold=%llu, heapDumpOnExit=true, heapDumpPath=%s", param.Threshold,
                   param.HeapDumpPath.c_str());
    } else {
        Log::Trace("Starting gc watchdog with params treshold=%llu, heapDumpOnExit=false", param.Threshold);
    }

    VM::Init(vm);

    const GcWatchdog& wd = param.IsHeapDumpEnabled ? GcWatchdog(param.Threshold, param.HeapDumpPath)
            : GcWatchdog(param.Threshold);

    std::thread _wd_t(wd);
    _wd_t.detach();

    return 0;
}

JNIEXPORT void JNICALL Agent_OnUnload(JavaVM *vm) {
    GcWatchdog::stop();
}
