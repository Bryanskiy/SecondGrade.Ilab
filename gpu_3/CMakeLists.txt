cmake_minimum_required(VERSION 3.10)
project(dirichlet)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

find_package(OpenCL REQUIRED)
find_package(GTest REQUIRED)

add_subdirectory(CLFramework)

add_executable(main main.cpp)
target_link_libraries(
    main
    CLFramework
    ${OpenCL_LIBRARY}
)

add_executable(execute_tests tests/execute_tests.cpp)
target_link_libraries(
    execute_tests
    CLFramework
    ${OpenCL_LIBRARY}
    GTest::GTest 
    GTest::Main
)
