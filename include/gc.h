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

#ifndef GCWATCHDOG_GC_H
#define GCWATCHDOG_GC_H

#include <thread>
#include <jni.h>
#include <jvmti.h>
#include "util.h"
#include "log.h"

static const int NANOS_IN_MILLISECOND = 1000000;

class GcEvent {
private:
    static volatile uint64_t _ts;
    static std::mutex _event_sync;
    static std::condition_variable _event_signal;
public:
    static bool awaitGcEvent(uint64_t timeout);
    static void JNICALL GarbageCollectionStart(__unused jvmtiEnv *jvmti);
    static void JNICALL GarbageCollectionFinish(jvmtiEnv *jvmti);
};

class GcWatchdog {
private:
    uint64_t _threshold;
    bool _heapDump;
    std::string _dumpPath = "";
    static std::atomic_bool _stop;
public:
    explicit GcWatchdog(uint64_t threshold): _threshold(threshold), _heapDump(false) {};
    explicit GcWatchdog(uint64_t threshold, std::string dump_path):
                _threshold(threshold), _heapDump(true), _dumpPath(std::move(dump_path)){};
    void operator()() const;
    static void stop();
};

#endif //GCWATCHDOG_GC_H
