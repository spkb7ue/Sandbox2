cmake_minimum_required (VERSION 2.6)
project(Rabbit CXX)
set(CMAKE_CXX_STANDARD 11)

file(GLOB Rabbit_Src "*.cpp")
file(GLOB Rabbit_Inc "*.h")

set(CMAKE_BUILD_TYPE release)
set(STATIC_LIBRARY_OUTPUT_DIR ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/lib)
set(STATIC_LIBRARY_HEADERS_DIR ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/include)

# Project uses BOOST Unit testing framework
find_package(Boost COMPONENTS system filesystem unit_test_framework REQUIRED)
add_definitions(-DBOOST_TEST_DYN_LINK)

add_library(Rabbit ${Rabbit_Src})
set_target_properties(Rabbit PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${STATIC_LIBRARY_OUTPUT_DIR})
file(COPY ${Rabbit_Inc} DESTINATION ${STATIC_LIBRARY_HEADERS_DIR})

add_subdirectory(examples)
add_subdirectory(tests)
