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

#include <cstring>
#include <utility>
#include <vector>
#include "param.h"

std::string error_msg(const char* fmt, ...) {
    char buf[128];

    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);

    std::string ret(buf);
    return ret;
}

std::pair<std::string, std::string> parse_kv(const char* kv) {
    char buf[1024];
    strlcpy(buf, kv, sizeof(buf));

    const char* sep = "=";
    char *saveptr;
    char* key = strtok_r(buf, sep, &saveptr);
    char* val = strtok_r(nullptr, sep, &saveptr);

    if (key == nullptr || val == nullptr)
        throw std::runtime_error(error_msg("Invalid input options, invalid key value pair %s", kv));

    std::string key_s(key);
    std::string val_s(val);

    return std::make_pair(key_s, val_s);
}

GcWatchdogParam parse(const char *input) {
    GcWatchdogParam ret;

    if (input == nullptr)
        return ret;

    std::vector<std::pair<std::string, std::string>> kv_list;

    char buf[1024];
    strlcpy(buf, input, sizeof(buf));

    char *tok, *saveptr;
    const char *sep = ",";
    tok = strtok_r(buf, sep, &saveptr);
    if (tok == nullptr)
        throw FormatException(error_msg("Invalid input options %s", input));

    while (tok != nullptr) {
        if (strcmp(tok, "help") == 0)
            throw HelpException();

        kv_list.push_back(parse_kv(tok));
        tok = strtok_r(nullptr, sep, &saveptr);
    }

    for(const auto& kv : kv_list) {
        auto key = kv.first;
        auto val = kv.second;

        if (key == "threshold") {
            ret.Threshold = strtoull(val.c_str(), nullptr, 10);
        } else if (key == "heapDumpOnExit") {
            std::transform(val.begin(), val.end(), val.begin(), [](unsigned char c){ return std::tolower(c); });
            ret.IsHeapDumpEnabled = val == "true";
        } else if (key == "heapDumpPath") {
            ret.HeapDumpPath = val;
        } else if (key == "logFilePath") {
            ret.LogFilePath = val;
        } else
            throw FormatException(error_msg("Invalid input options %s", input));
    }

    return ret;
}
