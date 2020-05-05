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

#ifndef GCWATCHDOG_PARAM_H
#define GCWATCHDOG_PARAM_H

#include <cstdint>
#include <string>
#include <ostream>

class FormatException: public std::runtime_error {
public:
    explicit FormatException(const std::string& msg) : std::runtime_error(msg) {};
};

class HelpException: public std::exception {};

struct GcWatchdogParam {
    uint64_t Threshold;
    bool IsHeapDumpEnabled;
    std::string HeapDumpPath;
    std::string LogFilePath;

    GcWatchdogParam(): Threshold(100), HeapDumpPath("."), IsHeapDumpEnabled(false), LogFilePath(""){};

    friend GcWatchdogParam parse(const char* input);
};

GcWatchdogParam parse(const char* input);

#endif //GCWATCHDOG_PARAM_H
