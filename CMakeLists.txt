cmake_minimum_required(VERSION 3.16)
project(gcwatchdog)

set(CMAKE_CXX_STANDARD 11)

find_package(Java REQUIRED)
find_package(JNI REQUIRED)
include(UseJava)

set(CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/extern/catch2)
add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})

set(JMM_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/extern/java)
include_directories(${JNI_INCLUDE_DIRS} ${JMM_INCLUDE_DIR} include)

add_subdirectory(src)
enable_testing()
add_subdirectory(test)

if(APPLE)
    set(UTIL_SOURCE src/util_macos.cpp)
else()
    set(UTIL_SOURCE src/util_linux.cpp)
endif()

set(SOURCE src/agent.cpp src/vm.cpp src/gc.cpp src/log.cpp src/param.cpp)

set(TEST_SOURCES test/main.cpp test/config_parse_test.cpp test/logger_test.cpp)
add_executable(tests ${UTIL_SOURCE} ${SOURCE} ${TEST_SOURCES})
target_link_libraries(tests Catch)

add_library(gcwatchdog SHARED  ${UTIL_SOURCE} ${SOURCE})