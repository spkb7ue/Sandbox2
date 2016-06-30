cmake_minimum_required(VERSION 2.8)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/tests)
include_directories(${PROJECT_SOURCE_DIR})

add_executable(TestMesh test_mesh.cpp)
target_link_libraries(TestMesh
                      ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
