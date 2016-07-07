cmake_minimum_required(VERSION 2.8)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/tests)
include_directories(${PROJECT_SOURCE_DIR})

add_executable(TestMesh test_mesh.cpp)
target_link_libraries(TestMesh
                      ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                      ${STATIC_LIBRARY_OUTPUT_DIR}/libRabbit.a)
add_custom_command(TARGET TestMesh PRE_BUILD COMMAND "${CMAKE_COMMAND}" -E copy
     "${CMAKE_SOURCE_DIR}/rabbit.triangles"
     "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/tests")

add_executable(TestVec3 test_vec3.cpp)
target_link_libraries(TestVec3
                      ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                      ${STATIC_LIBRARY_OUTPUT_DIR}/libRabbit.a)

add_executable(TestTriangle test_triangle.cpp)
target_link_libraries(TestTriangle
                      ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                      ${STATIC_LIBRARY_OUTPUT_DIR}/libRabbit.a)

add_executable(TestTriMeshQueryV1 test_tri_mesh_query_v1.cpp)
target_link_libraries(TestTriMeshQueryV1
                      ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                      ${STATIC_LIBRARY_OUTPUT_DIR}/libRabbit.a)

add_executable(TestAABB test_aabb.cpp)
target_link_libraries(TestAABB
                      ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                      ${STATIC_LIBRARY_OUTPUT_DIR}/libRabbit.a)
