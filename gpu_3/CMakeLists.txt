cmake_minimum_required(VERSION 3.10)
project(dirichlet)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

find_package(OpenCL REQUIRED)
find_package(GTest REQUIRED)

add_subdirectory(dirichlet)
add_subdirectory(cl_framework)

add_executable(main main.cpp)
target_link_libraries(
    main
    cl_framework
    dirichlet
    ${OpenCL_LIBRARY}
)

add_executable(execute_tests tests/execute_tests.cpp)
target_link_libraries(
    execute_tests
    dirichlet
    cl_framework
    ${OpenCL_LIBRARY}
    GTest::GTest 
    GTest::Main
)
