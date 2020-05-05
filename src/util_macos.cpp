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

#ifdef __APPLE__

#include <mach/mach_time.h>
#include <unistd.h>
#include "util.h"

static mach_timebase_info_data_t timebase = {0, 0};

uint64_t nanotime() {
    if (timebase.denom == 0) {
        mach_timebase_info(&timebase);
    }
    return (uint64_t)mach_absolute_time() * timebase.numer / timebase.denom;
}

timeval millis_t() {
    struct timeval tv{};
    gettimeofday(&tv, nullptr);
    return tv;
}

uint64_t millis() {
    struct timeval tv = millis_t();
    return (uint64_t)tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

uint32_t pid() {
    return getpid();
}

#endif
