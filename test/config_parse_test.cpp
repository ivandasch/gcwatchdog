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
#include "param.h"

TEST_CASE("Parsing options list") {
    SECTION("Parse null input") {
        auto res = parse(nullptr);

        REQUIRE(res.LogFilePath.empty());
        REQUIRE(res.HeapDumpPath == ".");
        REQUIRE_FALSE(res.IsHeapDumpEnabled);
        REQUIRE(res.Threshold == 100);
    }

    SECTION("Parse full input") {
        auto res = parse("threshold=1000,heapDumpOnExit=true,heapDumpPath=/tmp,logFilePath=/var/log/watchdog");

        REQUIRE(res.LogFilePath == "/var/log/watchdog");
        REQUIRE(res.HeapDumpPath == "/tmp");
        REQUIRE(res.IsHeapDumpEnabled);
        REQUIRE(res.Threshold == 1000);
    }

    SECTION("Parse partial input") {
        auto res = parse("threshold=2000,logFilePath=/var/log/watchdog");

        REQUIRE(res.LogFilePath == "/var/log/watchdog");
        REQUIRE(res.HeapDumpPath == ".");
        REQUIRE_FALSE(res.IsHeapDumpEnabled);
        REQUIRE(res.Threshold == 2000);
    }

    SECTION("Parse invalid input") {
        REQUIRE_THROWS_WITH(parse(","), Catch::Contains("Invalid input options"));
        REQUIRE_THROWS_WITH(parse("bla-bla-bla=erere"), Catch::Contains("Invalid input options"));
        REQUIRE_THROWS_WITH(parse("threshold,heapDumpOnExit=true,heap"), Catch::Contains("invalid key value pair threshold"));
    }

    SECTION("Help input") {
        REQUIRE_THROWS_AS(parse("help"), HelpException);
        REQUIRE_THROWS_AS(parse("help,threshold=1000"), HelpException);
    }
}
