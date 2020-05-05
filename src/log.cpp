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

#include <cstdarg>
#include <cstdio>
#include <iostream>
#include <fstream>
#include "util.h"
#include "log.h"

using namespace std::chrono;

std::mutex Log::_lock;
std::unique_ptr<std::ostream> Log::_os;

std::string str_time() {
    char time_buf[36], usec_buf[6];

    auto now = millis_t();
    auto sec = (time_t)now.tv_sec;

    strftime(time_buf, sizeof(time_buf), "%F %T", localtime(&sec));
    strcat(time_buf, ".");
    sprintf(usec_buf,"%d",(int)now.tv_usec / 1000);

    strcat(time_buf, usec_buf);

    std::string ret(time_buf);
    return ret;
}

void Log::Trace(const char *fmt, ...) {
    char buf[1024];

    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);

    std::lock_guard<std::mutex> guard(_lock);
    stream() << "[" << str_time() << "] " << buf << std::endl;
    stream().flush();
}

void Log::Init(const std::string& filename) {
    std::unique_ptr<std::ofstream> fos_ptr(new std::ofstream(filename, std::ios_base::out | std::ios_base::app));

    if (fos_ptr->is_open())
        _os = std::move(fos_ptr);
}

std::ostream &Log::stream() {
    return _os == nullptr ? std::cerr : *_os;
}
