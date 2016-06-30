cmake_minimum_required (VERSION 2.6)
project(Rabbit C CXX)
set(CMAKE_CXX_STANDARD 11)

file(GLOB Rabbit_Src "*.h" "*.cpp")

set(CMAKE_BUILD_TYPE debug)
set(STATIC_LIBRARY_OUTPUT_DIR ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/lib)
set(STATIC_LIBRARY_HEADERS_DIR ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/include)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/bin)

enable_testing()

find_package(Boost COMPONENTS system filesystem unit_test_framework REQUIRED)
add_definitions(-DBOOST_TEST_DYN_LINK)

add_executable(scratch ${Rabbit_Src})

add_custom_command(TARGET scratch PRE_BUILD COMMAND "${CMAKE_COMMAND}" -E copy
     "${CMAKE_SOURCE_DIR}/rabbit.triangles"
     "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/bin")
