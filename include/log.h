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

#ifndef GCWATCHDOG_LOG_H
#define GCWATCHDOG_LOG_H

#include <mutex>
#include <memory>
#include <iostream>

class Log {
private:
    static std::mutex _lock;
    static std::unique_ptr<std::ostream> _os;
    static std::ostream& stream();
public:
    static void Init(const std::string& filename);
    static void Trace(const char* fmt, ...);
};

#endif //GCWATCHDOG_LOG_H
