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

#include "catch.hpp"
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdio>
#include "log.h"

struct cerr_redirect {
    explicit cerr_redirect(std::streambuf *new_buffer): old(std::cerr.rdbuf(new_buffer)) {}

    ~cerr_redirect( ) {
        std::cerr.rdbuf(old);
    }
private:
    std::streambuf *old;
};

TEST_CASE("Logger test") {
    SECTION("Test cerr output") {
        std::stringstream buffer;
        cerr_redirect _redir(buffer.rdbuf());

        Log::Trace("Test output");

        REQUIRE_THAT(buffer.str(), Catch::Contains("Test output"));
    }

    SECTION("Test formatting") {
        std::stringstream buffer;
        cerr_redirect _redir(buffer.rdbuf());

        Log::Trace("Test format %d*%d = %d", 2, 2, 4);

        REQUIRE_THAT(buffer.str(), Catch::Contains("Test format 2*2 = 4"));
    }

    SECTION("Test file output") {
        auto path = "./test.log";
        remove(path);

        Log::Init(path);
        Log::Trace("Test output");

        std::ifstream _is(path);

        REQUIRE(_is.is_open());

        std::string line;
        std::getline(_is, line);

        REQUIRE_THAT(line, Catch::Contains("Test output"));
    }
}

