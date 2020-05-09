cmake_minimum_required(VERSION 3.9)
project(gcwatchdog)

set(CPACK_GENERATOR "TGZ")
include(CPack)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Java REQUIRED)
find_package(JNI REQUIRED)
find_package(Threads REQUIRED)
include(UseJava)

set(CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/extern/catch2)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/extern/catch2")
add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})

set(JMM_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/extern/java)
include_directories(${JNI_INCLUDE_DIRS} ${JMM_INCLUDE_DIR} include)

add_subdirectory(src)
enable_testing()
add_subdirectory(test)

set(TARGET gcwatchdog)
set(TEST_TARGET gcwatchdog_tests)

if(APPLE)
    set(UTIL_SOURCE src/util_macos.cpp)
else()
    set(UTIL_SOURCE src/util_linux.cpp)
endif()

set(SOURCE src/agent.cpp src/vm.cpp src/gc.cpp src/log.cpp src/param.cpp)

set(TEST_SOURCES test/main.cpp test/config_parse_test.cpp test/logger_test.cpp)
add_executable(${TEST_TARGET} ${UTIL_SOURCE} ${SOURCE} ${TEST_SOURCES})
target_link_libraries(${TEST_TARGET} Catch ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})

add_library(${TARGET} SHARED  ${UTIL_SOURCE} ${SOURCE})
target_link_libraries(${TARGET} ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})

include(CTest)
include(Catch)
catch_discover_tests(${TEST_TARGET})

install(TARGETS ${TARGET} DESTINATION ".")