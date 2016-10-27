cmake_minimum_required(VERSION 3.4)
project(thalliumvm)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Os -fomit-frame-pointer -fstrict-aliasing")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -std=c++14")

set(CMAKE_CXX_COMPILER "clang++")

set(SOURCE_FILES main.cpp thallium/vm.hpp thallium/vm.cpp thallium/instruction.hpp thallium/register.hpp thallium/register.cpp thallium/error.hpp thallium/error.cpp thallium/serializer.hpp)
add_executable(thalliumvm ${SOURCE_FILES})
