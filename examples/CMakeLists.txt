cmake_minimum_required(VERSION 2.8)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/examples)
include_directories(${PROJECT_SOURCE_DIR})

add_executable(MeshBuilderEx MeshBuilderEx.cpp)
target_link_libraries(MeshBuilderEx ${STATIC_LIBRARY_OUTPUT_DIR}/libRabbit.a)
add_custom_command(TARGET MeshBuilderEx PRE_BUILD COMMAND "${CMAKE_COMMAND}" -E copy
     "${CMAKE_SOURCE_DIR}/rabbit.triangles"
     "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/examples")

add_executable(ShapeEx ShapeEx.cpp)
target_link_libraries(ShapeEx ${STATIC_LIBRARY_OUTPUT_DIR}/libRabbit.a)
