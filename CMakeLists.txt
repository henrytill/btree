cmake_minimum_required(VERSION 3.16)
project(btree C)

set(CMAKE_C_STANDARD 99)

# library

set(btree_sources src/btree.c)

add_library(btree SHARED ${btree_sources})
target_include_directories(btree PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

# tests

find_package(OpenSSL REQUIRED)

add_executable(btree-test-basic test/test-basic.c)
target_include_directories(btree-test-basic PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
target_link_libraries(btree-test-basic btree OpenSSL::SSL)

enable_testing()
add_test(test_basic, btree-test-basic)
