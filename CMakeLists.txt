cmake_minimum_required(VERSION 3.16)
project(btree C)

set(CMAKE_C_STANDARD 99)

option(BTREE_COVERAGE "Compile with coverage support" ON)

if(BTREE_COVERAGE MATCHES "ON")
  SET(GCC_COVERAGE_COMPILE_FLAGS "-fprofile-arcs -ftest-coverage")
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
endif()

# library

set(btree_sources src/btree.c)

add_library(btree SHARED ${btree_sources})
target_include_directories(btree PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

# tests

find_package(OpenSSL REQUIRED)

add_executable(basic test/basic.c)
target_include_directories(basic PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
target_link_libraries(basic btree OpenSSL::SSL)

add_executable(get_path test/get_path.c)
target_include_directories(get_path PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>)
target_link_libraries(get_path btree OpenSSL::SSL)

add_executable(txn_abort test/txn_abort.c)
target_include_directories(txn_abort PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
target_link_libraries(txn_abort btree OpenSSL::SSL)

add_executable(flex_mem test/flex_mem.c)

enable_testing()
add_test(basic, basic)
add_test(get_path, get_path)
add_test(txn_abort, txn_abort)
add_test(flex_mem, flex_mem)
