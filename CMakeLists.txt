cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 14)

project(Json VERSION 1.0.0 LANGUAGES CXX)

file(GLOB_RECURSE CORE_SRCS "src/*.cpp")

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

find_package(genericlexer REQUIRED)
find_package(Threads REQUIRED)

add_library(fox_json ${CORE_SRCS})
target_include_directories(fox_json PUBLIC "include")
target_link_libraries(fox_json PRIVATE lexer Threads::Threads)

if (FOX_JSON_EXAMPLE)
    add_subdirectory(example)
endif()