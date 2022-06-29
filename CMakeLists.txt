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

add_executable(btree-test-basic test/test-basic.c test/test.h)
target_include_directories(btree-test-basic PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
target_link_libraries(btree-test-basic btree OpenSSL::SSL)

add_executable(btree-test-get-path test/test-get-path.c test/test.h)
target_include_directories(btree-test-get-path PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>)
target_link_libraries(btree-test-get-path btree OpenSSL::SSL)

add_executable(btree-test-txn-abort test/test-txn-abort.c test/test.h)
target_include_directories(btree-test-txn-abort PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
target_link_libraries(btree-test-txn-abort btree OpenSSL::SSL)

add_executable(misc-flexible-array-member test/misc-flexible-array-member.c test/misc-flexible-array-member.h)

enable_testing()
add_test(test_basic, btree-test-basic)
add_test(test_get_path, btree-test-get-path)
add_test(test_txn_abort, btree-test-txn-abort)
add_test(misc_flexible_array_member, misc-flexible-array-member)
