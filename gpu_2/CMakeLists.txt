cmake_minimum_required(VERSION 3.10)
project(patterns_matching)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_RELEASE "-O2")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -DLOG")

find_package(OpenCL REQUIRED)
find_package(Boost COMPONENTS program_options REQUIRED)

add_subdirectory(gpu_matching)
add_subdirectory(cpu_matching)


add_executable(main main.cpp)
target_link_libraries(
    main 
    gpu_matching
    cpu_matching
    ${OpenCL_LIBRARY}
    Boost::program_options
)



add_executable(time_tests tests/time_tests.cpp)
target_link_libraries(
    time_tests
    gpu_matching
    cpu_matching
    ${OpenCL_LIBRARY}
    Boost::program_options
)

add_executable(execute_tests tests/execute_tests.cpp)
target_link_libraries(
    execute_tests
    gpu_matching
    cpu_matching
    ${OpenCL_LIBRARY}
    Boost::program_options
)
