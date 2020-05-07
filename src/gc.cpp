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

#include <chrono>
#include <csignal>
#include <future>
#include "gc.h"
#include "log.h"
#include "vm.h"

std::atomic_ullong GcEvent::_ts(0L);
std::atomic_bool GcWatchdog::_stop(false);
std::condition_variable GcEvent::_event_signal;
std::mutex GcEvent::_event_sync;

void GcEvent::GarbageCollectionStart(jvmtiEnv *jvmti) {
    std::unique_lock<std::mutex> lock(_event_sync);
    _ts = nanotime();
    _event_signal.notify_all();
}

void GcEvent::GarbageCollectionFinish(jvmtiEnv *jvmti) {
    std::unique_lock<std::mutex> lock(_event_sync);
    _ts = 0L;
    _event_signal.notify_all();
}

bool GcEvent::awaitGcEvent(uint64_t timeout) {
    std::unique_lock<std::mutex> lock(_event_sync);
    if (_ts.load() == 0) {
        _event_signal.wait(lock, [&] { return _ts.load() > 0;});
        return true;
    } else {
        return _event_signal.wait_for(lock, std::chrono::milliseconds(timeout), [&] { return _ts.load() == 0; });
    }
}

void GcWatchdog::operator()() const {
    while(!_stop.load()) {
        if (!GcEvent::awaitGcEvent(_threshold)) {
            uint64_t duration = nanotime() - GcEvent::ts();
            Log::Trace("GC took time %lluns, more than threshold of %llums, killing JVM", duration, _threshold);

            if (_heapDump) {
                auto fut = std::async(std::launch::async,[&] {
                    Log::Trace("Taking heap dump of process[pid=%u] in path %s before SIGKILL", pid(), _dumpPath.c_str());
                    VM::HeapDump(_dumpPath.c_str());
                });

                fut.wait_for(std::chrono::seconds(30));
            }

            raise(SIGKILL);
        }
    }
}

void GcWatchdog::stop() {
    _stop = true;
}
